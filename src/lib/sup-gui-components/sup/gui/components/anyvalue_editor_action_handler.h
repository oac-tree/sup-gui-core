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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_ACTION_HANDLER_H_
#define SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_ACTION_HANDLER_H_

#include <sup/gui/components/anyvalue_editor_context.h>
#include <sup/gui/components/i_anyvalue_editor_action_handler.h>

#include <memory>

class QMimeData;

namespace sup::gui
{

class QueryResult;

/**
 * @brief The AnyValueEditorActionHandler class implements logic to manipulate AnyValue's from the
 * toolbar.
 *
 * It allows to add to the model AnyValueItems representing scalars, structs, and arrays.
 * The class relies on callbacks to query currently selected items and to report an error if the
 * action is not possible. Depending on the current selection, AnyValueItem can be added either as a
 * top-level item, or as a field to already existing items.
 *
 * Given implementation reflects the needs of AnyValueEditor where we edit a single top-level
 * AnyValueItem at a time. Thus, it is assumed that there is only a single top-level AnyValueItem
 * and it is located in its own container.
 */
class AnyValueEditorActionHandler : public IAnyValueEditorActionHandler
{
public:
  explicit AnyValueEditorActionHandler(AnyValueEditorContext context);
  AnyValueEditorActionHandler(AnyValueEditorContext context, mvvm::SessionItem* container);
  ~AnyValueEditorActionHandler() override;

  void SetAnyValueItemContainer(mvvm::SessionItem* container) override;

  bool CanInsertAfter(const std::string& type_name) const override;

  void InsertAnyValueItemAfter(const std::string& type_name) override;

  bool CanInsertInto(const std::string& type_name) const override;

  void InsertAnyValueItemInto(const std::string& type_name) override;

  bool CanRemove() const override;

  void RemoveSelected() override;

  void OnImportFromFileRequest(const std::string& file_name) override;

  void OnExportToFileRequest(const std::string& file_name) override;

  void MoveUp() override;

  void MoveDown() override;

  bool CanCut() const override;

  void Cut() override;

  bool CanCopy() const override;

  void Copy() override;

  bool CanPasteAfter() const override;

  void PasteAfter() override;

  bool CanPasteInto() const override;

  void PasteInto() override;

  void SetInitialValue(const AnyValueItem& item) override;

  AnyValueItem* GetTopItem() override;

  const AnyValueItem* GetTopItem() const override;

  bool CanUndo() const override;

  void Undo() override;

  bool CanRedo() const override;

  void Redo() override;

private:
  AnyValueItem* GetSelectedItem() const;

  std::vector<AnyValueItem*> GetSelectedItems() const;

  mvvm::SessionItem* GetAnyValueItemContainer() const;

  void RequestNotify(mvvm::SessionItem* item) const;

  mvvm::SessionItem* GetParentToInsert() const;

  mvvm::ISessionModel* GetModel() const;

  void SendMessage(const MessageEvent& message) const;

  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {});

  void InsertAfterCurrentSelection(std::vector<std::unique_ptr<mvvm::SessionItem>> items);

  void InsertIntoCurrentSelection(std::vector<std::unique_ptr<mvvm::SessionItem>> items);

  QueryResult CanInsertTypeAfterCurrentSelection(const std::string& item_type) const;

  QueryResult CanInsertTypeIntoCurrentSelection(const std::string& item_type) const;

  void InsertItem(std::vector<std::unique_ptr<mvvm::SessionItem>> items,
                                mvvm::SessionItem* parent_item, const mvvm::TagIndex& index);

  const QMimeData* GetClipboardContent() const;

  AnyValueEditorContext m_context;
  mvvm::SessionItem* m_container{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_ACTION_HANDLER_H_
