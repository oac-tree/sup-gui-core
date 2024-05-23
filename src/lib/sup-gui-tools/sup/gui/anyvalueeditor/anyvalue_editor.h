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

#ifndef SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_H_
#define SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_H_

#include <sup/gui/anyvalueeditor/anyvalue_editor_context.h>

#include <QString>
#include <QWidget>
#include <memory>

namespace mvvm
{
class ApplicationModel;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueEditorWidget;

class AnyValueEditor : public QWidget
{
  Q_OBJECT

public:
  explicit AnyValueEditor(QWidget* parent = nullptr);
  ~AnyValueEditor() override;

  void SetInitialValue(const sup::gui::AnyValueItem& item);

  AnyValueItem* GetTopItem();

  mvvm::ApplicationModel* GetModel() const;

  void OnImportFromFileRequest();
  void OnExportToFileRequest();
  void OnProjectLoad();

private:
  std::unique_ptr<mvvm::ApplicationModel> m_model;
  AnyValueEditorWidget* m_editor_widget{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_H_
