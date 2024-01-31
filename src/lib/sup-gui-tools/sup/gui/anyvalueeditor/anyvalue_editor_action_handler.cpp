/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>

namespace sup::gui
{

AnyValueEditorActionHandler::AnyValueEditorActionHandler(AnyValueEditorContext context,
                                                         mvvm::SessionItem* container,
                                                         QObject* parent)
    : QObject(parent), m_container(container), m_context(std::move(context))
{
}

AnyValueEditorActionHandler::~AnyValueEditorActionHandler() = default;

void AnyValueEditorActionHandler::OnAddEmptyAnyValue()
{
  AddAnyValueItem(std::make_unique<AnyValueEmptyItem>());
}

void AnyValueEditorActionHandler::OnAddAnyValueStruct()
{
  AddAnyValueItem(std::make_unique<AnyValueStructItem>());
}

void AnyValueEditorActionHandler::OnAddAnyValueArray()
{
  AddAnyValueItem(std::make_unique<AnyValueArrayItem>());
}

void AnyValueEditorActionHandler::OnAddAnyValueScalar(const std::string& scalar_type)
{
  if (auto array_item = dynamic_cast<sup::gui::AnyValueArrayItem*>(GetSelectedItem()); array_item)
  {
    if (!sup::gui::IsSuitableScalarType(*array_item, scalar_type))
    {
      SendMessage("Array element mismatch");
      return;
    }
  }

  auto item = std::make_unique<AnyValueScalarItem>();
  item->SetAnyTypeName(scalar_type);
  item->SetToolTip(scalar_type);
  AddAnyValueItem(std::move(item));
}

void AnyValueEditorActionHandler::OnRemoveSelected()
{
  if (auto selected = GetSelectedItem(); selected)
  {
    GetModel()->RemoveItem(selected);
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
  GetModel()->InsertItem(sup::gui::CreateItem(anyvalue), GetParent(), mvvm::TagIndex::Append());
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

void AnyValueEditorActionHandler::SetInitialValue(const AnyValueItem& item)
{
  if (auto item = GetTopItem(); item)
  {
    SendMessage("Only one top item is allowed");
    return;
  }

  GetModel()->InsertItem(mvvm::utils::CloneItem(item), GetAnyValueItemContainer(),
                         mvvm::TagIndex::Append());
}

AnyValueItem* AnyValueEditorActionHandler::GetTopItem()
{
  return mvvm::utils::GetTopItem<AnyValueItem>(GetModel());
}

AnyValueItem* AnyValueEditorActionHandler::GetSelectedItem() const
{
  return m_context.get_selected_callback ? m_context.get_selected_callback() : nullptr;
}

mvvm::SessionItem* AnyValueEditorActionHandler::GetAnyValueItemContainer() const
{
  return m_container;
}

mvvm::SessionItem* AnyValueEditorActionHandler::GetParent() const
{
  return GetSelectedItem() ? GetSelectedItem() : GetAnyValueItemContainer();
}

mvvm::SessionModelInterface* AnyValueEditorActionHandler::GetModel() const
{
  return m_container->GetModel();
}

void AnyValueEditorActionHandler::SendMessage(const std::string& text,
                                              const std::string& informative,
                                              const std::string& details)
{
  auto message = sup::gui::CreateInvalidOperationMessage(text, informative, details);
  m_context.send_message_callback(message);
}

void AnyValueEditorActionHandler::AddAnyValueItem(std::unique_ptr<AnyValueItem> item)
{
  if (!GetSelectedItem() && GetTopItem())
  {
    SendMessage("Please select an item where you want to add a field");
    return;
  }

  if (auto parent = GetParent(); parent)
  {
    try
    {
      if (auto name = SuggestDisplayName(*parent, *item); name.has_value())
      {
        item->SetDisplayName(name.value());
      }
      auto result = GetModel()->InsertItem(std::move(item), parent, mvvm::TagIndex::Append());
      emit SelectItemRequest(result);
    }
    catch (const std::exception& ex)
    {
      SendMessage("Can't add item to current selection", "", ex.what());
    }
  }
}

}  // namespace sup::gui
