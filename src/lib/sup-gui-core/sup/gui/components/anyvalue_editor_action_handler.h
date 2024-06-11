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

#ifndef SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_ACTION_HANDLER_H_
#define SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_ACTION_HANDLER_H_

#include "anyvalue_editor_context.h"

#include <QObject>
#include <memory>

namespace mvvm
{
class SessionItem;
class SessionModelInterface;
class TagIndex;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueItem;
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
class AnyValueEditorActionHandler : public QObject
{
  Q_OBJECT

public:
  AnyValueEditorActionHandler(AnyValueEditorContext context, mvvm::SessionItem* container,
                              QObject* parent);
  ~AnyValueEditorActionHandler() override;

  /**
   * @brief Sets an item that represents a container to store top-level AnyValueItem.
   */
  void SetAnyValueItemContainer(mvvm::SessionItem* container);

  /**
   * @brief Checks if AnyValueItem of the given type can be inserted after currently selected
   * AnyValueItem.
   */
  bool CanInsertAfter(const std::string &item_type) const;

  /**
   * @brief Inserts new AnyValueItem of the given type after current selection.
   */
  void OnInsertAnyValueItemAfter(const std::string& type_name);

  /**
   * @brief Checks if AnyValueItem of the given type can be inserted into currently selected
   * AnyValueItem.
   */
  bool CanInsertInto(const std::string &item_type) const;

  /**
   * @brief Inserts new AnyValueItem of the given type into current selection.
   */
  void OnInsertAnyValueItemInto(const std::string& type_name);

  /**
   * @brief Removes currently selected item.
   */
  void OnRemoveSelected();

  /**
   * @brief Imports AnyValue from file.
   *
   * Depending on the current selection, it can become either a top-level item, a field in a
   * structure, or array element.
   */
  void OnImportFromFileRequest(const std::string& file_name);

  /**
   * @brief Exports top-level AnyValue to file.
   */
  void OnExportToFileRequest(const std::string& file_name);

  /**
   * @brief Moves a structure field, or array element up.
   */
  void OnMoveUpRequest();

  /**
   * @brief Moves a structure field, or array element down.
   */
  void OnMoveDownRequest();

  /**
   * @brief Sets initial value.
   *
   * The given value will be cloned inside the editor's model and used as a starting point for
   * editing.
   *
   * @param item The value to set.
   */
  void SetInitialValue(const AnyValueItem& item);

  /**
   * @brief Returns a top-level AnyValueItem (i.e. the result of the editing).
   */
  AnyValueItem* GetTopItem();

  /**
   * @brief Returns a top-level AnyValueItem (i.e. the result of the editing).
   */
  const AnyValueItem* GetTopItem() const;

  /**
   * @brief Returns currently selected item.
   */
  sup::gui::AnyValueItem* GetSelectedItem() const;

  /**
   * @brief Returns container used to store the top-level AnyValueItem.
   */
  mvvm::SessionItem* GetAnyValueItemContainer() const;

signals:
  void SelectItemRequest(mvvm::SessionItem* item);

private:
  /**
   * @brief Returns parent to use while inserting an item.
   */
  mvvm::SessionItem* GetParent() const;

  /**
   * @brief Returns the model.
   */
  mvvm::SessionModelInterface* GetModel() const;

  void SendMessage(const sup::gui::MessageEvent& message);

  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {});

  /**
   * @brief Inserts given item after currenly selected item.
   */
  void InsertAfterCurrentSelection(std::unique_ptr<AnyValueItem> item);

  /**
   * @brief Inserts given item into currenly selected item.
   */
  void InsertIntoCurrentSelection(std::unique_ptr<AnyValueItem> item);

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
  mvvm::SessionItem* InsertItem(std::unique_ptr<mvvm::SessionItem> item, mvvm::SessionItem* parent,
                                const mvvm::TagIndex& index);

  AnyValueEditorContext m_context;
  mvvm::SessionItem* m_container{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_ACTION_HANDLER_H_
