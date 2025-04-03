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

#ifndef SUP_GUI_COMPONENTS_I_ANYVALUE_EDITOR_ACTION_HANDLER_H_
#define SUP_GUI_COMPONENTS_I_ANYVALUE_EDITOR_ACTION_HANDLER_H_

#include <string>

namespace mvvm
{
class SessionItem;
class ISessionModel;
class TagIndex;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueItem;

/**
 * @brief The IAnyValueEditorActionHandler class is an interface to to manipulate AnyValue's from
 * the toolbar.
 */
class IAnyValueEditorActionHandler
{
public:
  IAnyValueEditorActionHandler() = default;
  virtual ~IAnyValueEditorActionHandler() = default;

  IAnyValueEditorActionHandler(const IAnyValueEditorActionHandler&) = delete;
  IAnyValueEditorActionHandler& operator=(const IAnyValueEditorActionHandler&) = delete;
  IAnyValueEditorActionHandler(IAnyValueEditorActionHandler&&) = delete;
  IAnyValueEditorActionHandler& operator=(IAnyValueEditorActionHandler&&) = delete;

  /**
   * @brief Sets an item that represents a container to store top-level AnyValueItem.
   */
  virtual void SetAnyValueItemContainer(mvvm::SessionItem* container) = 0;

  /**
   * @brief Checks if AnyValueItem with the given type name can be inserted after currently selected
   * AnyValueItem.
   *
   * Method uses AnyValueItem type names as provided by menu labels ("struct", "int32", etc...)
   *
   * @param type_name The type name.
   */
  virtual bool CanInsertAfter(const std::string& type_name) const = 0;

  /**
   * @brief Inserts new AnyValueItem of the given type after current selection.
   */
  virtual void OnInsertAnyValueItemAfter(const std::string& type_name) = 0;

  /**
   * @brief Checks if AnyValueItem with the given type name can be inserted into currently selected
   * AnyValueItem.
   *
   * Method uses AnyValueItem type names as provided by menu labels ("struct", "int32", etc...)
   *
   * @param type_name The type name.
   */
  virtual bool CanInsertInto(const std::string& type_name) const = 0;

  /**
   * @brief Inserts new AnyValueItem of the given type into current selection.
   */
  virtual void OnInsertAnyValueItemInto(const std::string& type_name) = 0;

  /**
   * @brief Checks if currently selected AnyValue can be removed.
   */
  virtual bool CanRemove() const = 0;

  /**
   * @brief Removes currently selected item.
   */
  virtual void OnRemoveSelected() = 0;

  /**
   * @brief Imports AnyValue from file.
   *
   * Depending on the current selection, it can become either a top-level item, a field in a
   * structure, or array element.
   */
  virtual void OnImportFromFileRequest(const std::string& file_name) = 0;

  /**
   * @brief Exports top-level AnyValue to file.
   */
  virtual void OnExportToFileRequest(const std::string& file_name) = 0;

  /**
   * @brief Moves a structure field, or array element up.
   */
  virtual void OnMoveUpRequest() = 0;

  /**
   * @brief Moves a structure field, or array element down.
   */
  virtual void OnMoveDownRequest() = 0;

  /**
   * @brief Checks if cut operation is possible.
   */
  virtual bool CanCut() const = 0;

  /**
   * @brief Cut selected AnyValue.
   */
  virtual void Cut() = 0;

  /**
   * @brief Checks if copy operation is possible.
   */
  virtual bool CanCopy() const = 0;

  /**
   * @brief Copy selected AnyValue.
   */
  virtual void Copy() = 0;

  /**
   * @brief Checks if paste-after operation is possible.
   *
   * Paste-after operation inserts new item after the current selection. If current selection
   * represent a field of a struct, a new field will be inserted right after. If current selection
   * represent an array element, the new element will be inserted right after.
   *
   * Paste-after operation is not possible for top-level items, since the editor allows to have only
   * a single top-level item.
   */
  virtual bool CanPasteAfter() const = 0;

  /**
   * @brief Paste new AnyValue after the current selection.
   */
  virtual void PasteAfter() = 0;

  /**
   * @brief Checks if paste-into operation is possible.
   *
   * Paste-into operation appends a new child to existing children of currently selected
   * AnyValue.
   *
   * If the current selection represents a struct, pasted AnyValue will be appended as a new field
   * to the list of existing fields. If the current selection represents an array, pasted AnyValue
   * will be appended to the list of current array elements.
   *
   * It is not possible to paste-into a scalar.
   */
  virtual bool CanPasteInto() const = 0;

  /**
   * @brief Paste AnyValue as a child into currently selected AnyValue.
   */
  virtual void PasteInto() = 0;

  /**
   * @brief Sets initial value.
   *
   * The given value will be cloned inside the editor's model and used as a starting point for
   * editing.
   *
   * @param item The value to set.
   */
  virtual void SetInitialValue(const AnyValueItem& item) = 0;

  /**
   * @brief Returns a top-level AnyValueItem (i.e. the result of the editing).
   */
  virtual AnyValueItem* GetTopItem() = 0;

  /**
   * @brief Returns a top-level AnyValueItem (i.e. the result of the editing).
   */
  virtual const AnyValueItem* GetTopItem() const = 0;

  /**
   * @brief Returns currently selected item.
   */
  virtual sup::gui::AnyValueItem* GetSelectedItem() const = 0;

  /**
   * @brief Returns container used to store the top-level AnyValueItem.
   */
  virtual mvvm::SessionItem* GetAnyValueItemContainer() const = 0;

  /**
   * @brief Checks if it is possible to undo last change.
   */
  virtual bool CanUndo() const = 0;

  /**
   * @brief Unddo last change.
   */
  virtual void Undo() = 0;

  /**
   * @brief Checks if it is possible to redo last change.
   */
  virtual bool CanRedo() const = 0;

  /**
   * @brief Redo last undo.
   */
  virtual void Redo() = 0;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_I_ANYVALUE_EDITOR_ACTION_HANDLER_H_
