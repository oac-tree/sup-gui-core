/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#ifndef SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TEXTPANEL_H_
#define SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TEXTPANEL_H_

#include <QWidget>

class QTextEdit;

namespace mvvm
{
class ApplicationModel;
class ModelHasChangedController;
}  // namespace mvvm

namespace sup::gui
{

//! Collapsible panel on the right of AnyValueEditor with json representation of AnyValueItem being
//! edited.

class AnyValueEditorTextPanel : public QWidget
{
  Q_OBJECT

public:
  explicit AnyValueEditorTextPanel(mvvm::ApplicationModel* model, QWidget* parent = nullptr);
  ~AnyValueEditorTextPanel();

private:
  void UpdateJson();

  QTextEdit* m_text_edit{nullptr};
  mvvm::ApplicationModel* m_model{nullptr};
  std::unique_ptr<mvvm::ModelHasChangedController> m_model_changed_controller;
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_TEXTPANEL_H_
