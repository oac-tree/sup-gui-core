/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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
 * It allows to add to the model AnyValueItems representing scalars, structs, and arrays. Depending
 * on passed parameters, items can be added either as top-level items, or as a field to already
 * existing items. The class rely on callbacks to query currently selected item and to report an
 * error if the action is not possible.
 */

class AnyValueEditorActionHandler : public QObject
{
  Q_OBJECT

public:
  AnyValueEditorActionHandler(AnyValueEditorContext context, mvvm::ApplicationModel* model,
                              QObject* parent);

  void OnAddEmptyAnyValue();

  void OnAddAnyValueStruct();

  void OnAddAnyValueArray();

  void OnAddAnyValueScalar(const std::string& scalar_type);

  void OnRemoveSelected();

  void OnImportFromFileRequest(const std::string& file_name);

  void OnExportToFileRequest(const std::string& file_name);

  void SetInitialValue(const AnyValueItem& item);

  AnyValueItem* GetTopItem();

  sup::gui::AnyValueItem* GetSelectedItem() const;

signals:
  void SelectItemRequest(mvvm::SessionItem* item);

private:
  mvvm::SessionItem* GetParent() const;
  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {});

  void AddAnyValueItem(std::unique_ptr<AnyValueItem> item);

  mvvm::ApplicationModel* m_model{nullptr};
  AnyValueEditorContext m_context;
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTION_HANDLER_H_
