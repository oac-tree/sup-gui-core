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

#ifndef SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTION_HANDLER_H_
#define SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTION_HANDLER_H_

#include <sup/gui/anyvalueeditor/anyvalue_editor_context.h>

#include <mvvm/model/application_model.h>

#include <QObject>

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{

class AnyValueItem;

/**
 * @brief The AnyValueEditorActionHandler class implements logic to manipulate AnyValue's from the
 * toolbar.
 *
 * @details It allows to add to the model AnyValueItems representing scalars, structs, and arrays.
 * The class rely on callbacks to query currently selected item and to report an error if the action
 * is not possible. Depending on current selection, AnyValueItem can be added either as top-level
 * item, or as a field to already existing items.
 *
 * @details Given implementation reflects needs of AnyValueEditor where we edit a single top level
 * AnyValueItem at a time. Thus, it is assumed that there is only signle top level AnyValueItem and
 * it is located in own container.
 */

class AnyValueEditorActionHandler : public QObject
{
  Q_OBJECT

public:
  AnyValueEditorActionHandler(AnyValueEditorContext context, mvvm::SessionItem* container,
                              QObject* parent);

  void OnAddEmptyAnyValue();

  void OnAddAnyValueStruct();

  void OnAddAnyValueArray();

  void OnAddAnyValueScalar(const std::string& scalar_type);

  void OnRemoveSelected();

  void OnImportFromFileRequest(const std::string& file_name);

  void OnExportToFileRequest(const std::string& file_name);

  void OnMoveUpRequest();

  void OnMoveDownRequest();

  /**
   * @brief Sets initial value.
   *
   * @param item The value to set.
   *
   * @details The given value will be cloned inside the editor's model and used as a starting point
   * for editing.
   */
  void SetInitialValue(const AnyValueItem& item);

  AnyValueItem* GetTopItem();

  sup::gui::AnyValueItem* GetSelectedItem() const;

  /**
   * @brief Returns container used to store top level AnyValueItem.
   */
  mvvm::SessionItem* GetAnyValueItemContainer() const;

signals:
  void SelectItemRequest(mvvm::SessionItem* item);

private:
  /**
   * @brief Returns parent to use while inserting an item
   */
  mvvm::SessionItem* GetParent() const;

  /**
   * @brief Returns the model.
   */
  mvvm::SessionModelInterface* GetModel() const;

  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {});

  void AddAnyValueItem(std::unique_ptr<AnyValueItem> item);

  AnyValueEditorContext m_context;
  mvvm::SessionItem* m_container{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTION_HANDLER_H_
