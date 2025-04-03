/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalue_editor_action_handler.h"

#include "anyvalue_editor_helper.h"
#include "mime_conversion_helper.h"

#include <sup/gui/core/exceptions.h>
#include <sup/gui/core/query_result.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/i_session_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/validate_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>

#include <QMimeData>
#include <sstream>

namespace
{
const std::string kFailedActionTitle = "Invalid Operation";
}

namespace sup::gui
{

AnyValueEditorActionHandler::AnyValueEditorActionHandler(AnyValueEditorContext context,
                                                         QObject* parent)
    : AnyValueEditorActionHandler(context, nullptr, parent)
{
}

AnyValueEditorActionHandler::AnyValueEditorActionHandler(AnyValueEditorContext context,
                                                         mvvm::SessionItem* container,
                                                         QObject* parent)
    : QObject(parent), m_context(std::move(context)), m_container(container)
{
}

AnyValueEditorActionHandler::~AnyValueEditorActionHandler() = default;

void AnyValueEditorActionHandler::SetAnyValueItemContainer(mvvm::SessionItem* container)
{
  m_container = container;
}

bool AnyValueEditorActionHandler::CanInsertAfter(const std::string& type_name) const
{
  return CanInsertTypeAfterCurrentSelection(GetAnyValueItemTypeFromTypeName(type_name)).IsSuccess();
}

void AnyValueEditorActionHandler::OnInsertAnyValueItemAfter(const std::string& type_name)
{
  auto query = CanInsertTypeAfterCurrentSelection(GetAnyValueItemTypeFromTypeName(type_name));

  if (!query.IsSuccess())
  {
    SendMessage(query.GetMessage());
    return;
  }

  auto result = CreateAnyValueItemFromTypeName(type_name);
  result->SetToolTip(type_name);
  InsertAfterCurrentSelection(std::move(result));
}

bool AnyValueEditorActionHandler::CanInsertInto(const std::string& type_name) const
{
  return CanInsertTypeIntoCurrentSelection(GetAnyValueItemTypeFromTypeName(type_name)).IsSuccess();
}

void AnyValueEditorActionHandler::OnInsertAnyValueItemInto(const std::string& type_name)
{
  auto query = CanInsertTypeIntoCurrentSelection(GetAnyValueItemTypeFromTypeName(type_name));
  if (!query.IsSuccess())
  {
    SendMessage(query.GetMessage());
    return;
  }

  auto result = CreateAnyValueItemFromTypeName(type_name);
  result->SetToolTip(type_name);
  InsertIntoCurrentSelection(std::move(result));
}

bool AnyValueEditorActionHandler::CanRemove() const
{
  return GetSelectedItem() != nullptr;
}

void AnyValueEditorActionHandler::OnRemoveSelected()
{
  if (auto selected = GetSelectedItem(); selected)
  {
    auto next_to_select = mvvm::utils::FindNextSiblingToSelect(selected);
    GetModel()->RemoveItem(selected);
    if (next_to_select)
    {
      // suggest to select something else instead of just deleted instruction
      emit SelectItemRequest(next_to_select);
    }
  }
}

void AnyValueEditorActionHandler::OnImportFromFileRequest(const std::string& file_name)
{
  if (!GetSelectedItem() && GetTopItem())
  {
    SendMessage("Please select an item where you want to add a field");
    return;
  }

  auto anyvalue = sup::gui::AnyValueFromJSONFile(file_name);
  auto item = sup::gui::CreateItem(anyvalue);
  if (auto query = mvvm::utils::CanInsertItem(item.get(), GetParent(), mvvm::TagIndex::Append());
      !query.first)
  {
    SendMessage(query.second);
    return;
  }
  GetModel()->InsertItem(std::move(item), GetParent(), mvvm::TagIndex::Append());
}

void AnyValueEditorActionHandler::OnExportToFileRequest(const std::string& file_name)
{
  if (!GetTopItem())
  {
    SendMessage("Nothing to save");
    return;
  }

  try
  {
    auto anyvalue = CreateAnyValue(*GetTopItem());
    sup::dto::AnyValueToJSONFile(anyvalue, file_name, /*pretty*/ true);
  }
  catch (const std::exception& ex)
  {
    SendMessage("Can't generate valid JSON presentation from current item", "Exception was thrown",
                ex.what());
  }
}

void AnyValueEditorActionHandler::OnMoveUpRequest()
{
  if (auto item = GetSelectedItem(); item)
  {
    if (mvvm::utils::MoveUp(*item))
    {
      emit SelectItemRequest(item);
    }
  }
}

void AnyValueEditorActionHandler::OnMoveDownRequest()
{
  if (auto item = GetSelectedItem(); item)
  {
    if (mvvm::utils::MoveDown(*item))
    {
      emit SelectItemRequest(item);
    }
  }
}

bool AnyValueEditorActionHandler::CanCut() const
{
  return CanRemove();
}

void AnyValueEditorActionHandler::Cut()
{
  if (!CanCut())
  {
    return;
  }

  Copy();
  OnRemoveSelected();
}

bool AnyValueEditorActionHandler::CanCopy() const
{
  const bool container_is_set = GetAnyValueItemContainer() != nullptr;
  const bool has_selection = GetSelectedItem() != nullptr;
  const bool has_clipboard = static_cast<bool>(m_context.set_mime_data);
  return container_is_set && has_selection && has_clipboard;
}

void AnyValueEditorActionHandler::Copy()
{
  if (!CanCopy())
  {
    return;
  }

  m_context.set_mime_data(sup::gui::CreateCopyMimeData(*GetSelectedItem(), kCopyAnyValueMimeType));
}

bool AnyValueEditorActionHandler::CanPasteAfter() const
{
  auto query =
      CanInsertTypeAfterCurrentSelection(GetSessionItemType(GetMimeData(), kCopyAnyValueMimeType));
  return query.IsSuccess();
}

void AnyValueEditorActionHandler::PasteAfter()
{
  if (!CanPasteAfter())
  {
    return;
  }

  InsertAfterCurrentSelection(sup::gui::CreateSessionItem(GetMimeData(), kCopyAnyValueMimeType));
}

bool AnyValueEditorActionHandler::CanPasteInto() const
{
  auto query =
      CanInsertTypeIntoCurrentSelection(GetSessionItemType(GetMimeData(), kCopyAnyValueMimeType));
  return query.IsSuccess();
}

void AnyValueEditorActionHandler::PasteInto()
{
  if (!CanPasteInto())
  {
    return;
  }

  InsertIntoCurrentSelection(sup::gui::CreateSessionItem(GetMimeData(), kCopyAnyValueMimeType));
}

void AnyValueEditorActionHandler::SetInitialValue(const AnyValueItem& item)
{
  if (auto top_item = GetTopItem(); top_item)
  {
    SendMessage("Only one top item is allowed");
    return;
  }

  auto cloned_item = mvvm::utils::CloneItem(item);

  // if original item was marked as disabled, we should remove it from clone
  cloned_item->SetEnabled(true);
  cloned_item->SetEditable(true);

  GetModel()->InsertItem(std::move(cloned_item), GetAnyValueItemContainer(),
                         mvvm::TagIndex::Append());
}

AnyValueItem* AnyValueEditorActionHandler::GetTopItem()
{
  return const_cast<AnyValueItem*>(
      static_cast<const AnyValueEditorActionHandler*>(this)->GetTopItem());
}

const AnyValueItem* AnyValueEditorActionHandler::GetTopItem() const
{
  if (GetAnyValueItemContainer() && GetAnyValueItemContainer()->GetTotalItemCount() > 0)
  {
    return GetAnyValueItemContainer()->GetItem<AnyValueItem>(mvvm::TagIndex::First());
  }
  return nullptr;
}

AnyValueItem* AnyValueEditorActionHandler::GetSelectedItem() const
{
  return m_context.selected_items ? m_context.selected_items() : nullptr;
}

mvvm::SessionItem* AnyValueEditorActionHandler::GetAnyValueItemContainer() const
{
  return m_container;
}

bool AnyValueEditorActionHandler::CanUndo() const
{
  auto command_stack = GetModel() ? GetModel()->GetCommandStack() : nullptr;
  return command_stack && command_stack->CanUndo();
}

void AnyValueEditorActionHandler::Undo()
{
  if (!CanUndo())
  {
    return;
  }

  GetModel()->GetCommandStack()->Undo();
}

bool AnyValueEditorActionHandler::CanRedo() const
{
  auto command_stack = GetModel() ? GetModel()->GetCommandStack() : nullptr;
  return command_stack && command_stack->CanRedo();
}

void AnyValueEditorActionHandler::Redo()
{
  if (!CanRedo())
  {
    return;
  }

  GetModel()->GetCommandStack()->Redo();
}

mvvm::SessionItem* AnyValueEditorActionHandler::GetParent() const
{
  return GetSelectedItem() ? GetSelectedItem() : GetAnyValueItemContainer();
}

mvvm::ISessionModel* AnyValueEditorActionHandler::GetModel() const
{
  return m_container->GetModel();
}

void AnyValueEditorActionHandler::SendMessage(const MessageEvent& message)
{
  m_context.send_message(message);
}

void AnyValueEditorActionHandler::SendMessage(const std::string& text,
                                              const std::string& informative,
                                              const std::string& details)
{
  SendMessage(sup::gui::CreateInvalidOperationMessage(text, informative, details));
}

void AnyValueEditorActionHandler::InsertAfterCurrentSelection(
    std::unique_ptr<mvvm::SessionItem> item)
{
  auto selected_item = GetSelectedItem();

  auto parent_item = selected_item ? selected_item->GetParent() : GetAnyValueItemContainer();
  auto tagindex = selected_item ? selected_item->GetTagIndex().Next() : mvvm::TagIndex::Append();

  UpdateChildAppearance(*parent_item, *item);
  (void)InsertItem(std::move(item), parent_item, tagindex);
}

void AnyValueEditorActionHandler::InsertIntoCurrentSelection(
    std::unique_ptr<mvvm::SessionItem> item)
{
  if (auto parent_item = GetParent(); parent_item)
  {
    UpdateChildAppearance(*parent_item, *item);
    (void)InsertItem(std::move(item), GetParent(), mvvm::TagIndex::Append());
  }
}

QueryResult AnyValueEditorActionHandler::CanInsertTypeAfterCurrentSelection(
    const std::string& item_type) const
{
  static const std::string kFailedActionText(
      "Can't insert AnyValue of given type after current selection");

  if (!GetAnyValueItemContainer())
  {
    return sup::gui::QueryResult::Failure(
        {kFailedActionTitle, kFailedActionText, "No container exists", ""});
  }

  const bool top_item_exists = GetTopItem() != nullptr;
  const bool top_item_selected = GetSelectedItem() && GetSelectedItem() == GetTopItem();
  const bool no_selection = GetSelectedItem() == nullptr;
  if (top_item_exists && (top_item_selected || no_selection))
  {
    return sup::gui::QueryResult::Failure(
        {kFailedActionTitle, kFailedActionText, "There can be only one top-level item", ""});
  }

  if (item_type.empty())
  {
    return sup::gui::QueryResult::Failure(
        {kFailedActionTitle, kFailedActionText, "Wrong item type [" + item_type + "]", ""});
  }

  // Checking if there is a selection inside another parent. To paste after this selection, the
  // parent should have the room for more items.
  if (auto selected_item = GetSelectedItem(); selected_item)
  {
    auto [success_flag, informative] = mvvm::utils::CanInsertType(
        item_type, selected_item->GetParent(), selected_item->GetTagIndex().Next());
    if (!success_flag)
    {
      return sup::gui::QueryResult::Failure(
          {kFailedActionTitle, kFailedActionText, informative, ""});
    }
  }

  return sup::gui::QueryResult::Success();
}

QueryResult AnyValueEditorActionHandler::CanInsertTypeIntoCurrentSelection(
    const std::string& item_type) const
{
  static const std::string kFailedActionText(
      "Can't insert AnyValue of given type into current selection");

  if (!GetAnyValueItemContainer())
  {
    return sup::gui::QueryResult::Failure(
        {kFailedActionTitle, kFailedActionText, "No container exists", ""});
  }

  if (!GetSelectedItem())
  {
    return sup::gui::QueryResult::Failure(
        {kFailedActionTitle, kFailedActionText, "No item selected", ""});
  }

  if (item_type.empty())
  {
    return sup::gui::QueryResult::Failure(
        {kFailedActionTitle, kFailedActionText, "Wrong item type [" + item_type + "]", ""});
  }

  // Checking if there is a selection inside another parent. To paste after this selection, the
  // parent should have the room for more items.
  if (auto selected_item = GetSelectedItem(); selected_item)
  {
    auto [success_flag, informative] =
        mvvm::utils::CanInsertType(item_type, selected_item, mvvm::TagIndex::Append());
    if (!success_flag)
    {
      return sup::gui::QueryResult::Failure(
          {kFailedActionTitle, kFailedActionText, informative, ""});
    }
  }

  return sup::gui::QueryResult::Success();
}

mvvm::SessionItem* AnyValueEditorActionHandler::InsertItem(std::unique_ptr<mvvm::SessionItem> item,
                                                           mvvm::SessionItem* parent_item,
                                                           const mvvm::TagIndex& index)
{
  if (!GetModel())
  {
    throw RuntimeException("Uninitialised model");
  }

  if (parent_item == nullptr)
  {
    throw RuntimeException("Uninitialised parent");
  }

  mvvm::SessionItem* result{nullptr};
  const auto item_type = item->GetType();
  try
  {
    result = GetModel()->InsertItem(std::move(item), parent_item, index);
    emit SelectItemRequest(result);
  }
  catch (const std::exception& ex)
  {
    std::ostringstream ostr;
    ostr << "Can't insert child [" << item_type << "] into parent [" << parent_item->GetType()
         << "].";
    SendMessage(ostr.str());
  }
  return result;
}

const QMimeData* AnyValueEditorActionHandler::GetMimeData() const
{
  return m_context.get_mime_data ? m_context.get_mime_data() : nullptr;
}

}  // namespace sup::gui
