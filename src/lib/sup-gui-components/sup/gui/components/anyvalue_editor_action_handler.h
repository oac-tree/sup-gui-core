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
  AnyValueEditorActionHandler(AnyValueEditorContext context);
  AnyValueEditorActionHandler(AnyValueEditorContext context, mvvm::SessionItem* container);
  ~AnyValueEditorActionHandler() override;

  void SetAnyValueItemContainer(mvvm::SessionItem* container) override;

  bool CanInsertAfter(const std::string& type_name) const override;

  void OnInsertAnyValueItemAfter(const std::string& type_name) override;

  bool CanInsertInto(const std::string& type_name) const override;

  void OnInsertAnyValueItemInto(const std::string& type_name) override;

  bool CanRemove() const override;

  void OnRemoveSelected() override;

  void OnImportFromFileRequest(const std::string& file_name) override;

  void OnExportToFileRequest(const std::string& file_name) override;

  void OnMoveUpRequest() override;

  void OnMoveDownRequest() override;

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

  sup::gui::AnyValueItem* GetSelectedItem() const override;

  mvvm::SessionItem* GetAnyValueItemContainer() const override;

  bool CanUndo() const override;

  void Undo() override;

  bool CanRedo() const override;

  void Redo() override;

private:
  void RequestNotify(mvvm::SessionItem* item);

  /**
   * @brief Returns parent to use while inserting an item.
   */
  mvvm::SessionItem* GetParent() const;

  /**
   * @brief Returns the model.
   */
  mvvm::ISessionModel* GetModel() const;

  void SendMessage(const sup::gui::MessageEvent& message);

  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {});

  /**
   * @brief Inserts given item after currenly selected item.
   */
  void InsertAfterCurrentSelection(std::unique_ptr<mvvm::SessionItem> item);

  /**
   * @brief Inserts given item into currenly selected item.
   */
  void InsertIntoCurrentSelection(std::unique_ptr<mvvm::SessionItem> item);

  /**
   * @brief Checks if AnyValueItem of the given type can be inserted after the current selection.
   *
   * @param item_type Type of the AnyValueItem to insert.
   * @return A result of the query with an error flag and message.
   */
  sup::gui::QueryResult CanInsertTypeAfterCurrentSelection(const std::string& item_type) const;

  /**
   * @brief Checks if AnyValueItem of the given type can be inserted into the current selection.
   *
   * @param item_type Type of the AnyValueItem to insert.
   * @return A result of the query with an error flag and message.
   */
  sup::gui::QueryResult CanInsertTypeIntoCurrentSelection(const std::string& item_type) const;

  /**
   * @brief Inserts given item in the given parent.
   */
  mvvm::SessionItem* InsertItem(std::unique_ptr<mvvm::SessionItem> item,
                                mvvm::SessionItem* parent_item, const mvvm::TagIndex& index);

  /**
   * @brief Returns mime data stored in the clipboar.
   */
  const QMimeData* GetMimeData() const;

  AnyValueEditorContext m_context;
  mvvm::SessionItem* m_container{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_ACTION_HANDLER_H_
