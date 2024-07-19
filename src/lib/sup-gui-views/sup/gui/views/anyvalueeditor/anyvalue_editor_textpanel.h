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

#ifndef SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_TEXTPANEL_H_
#define SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_TEXTPANEL_H_

#include <QWidget>

class QTextEdit;
class QAction;
class QToolButton;

namespace mvvm
{
class ISessionModel;
template <typename T>
class ModelListener;
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{

class CodeView;
class VisibilityAgentBase;
class AnyValueItem;

//! Collapsible panel on the right of AnyValueEditor with JSON representation of AnyValue.

class AnyValueEditorTextPanel : public QWidget
{
  Q_OBJECT

public:
  using listener_t = mvvm::ModelListener<mvvm::ISessionModel>;

  explicit AnyValueEditorTextPanel(mvvm::ISessionModel* model, QWidget* parent = nullptr);
  ~AnyValueEditorTextPanel() override;

  /**
   * @brief Sets the container with AnyValueItem.
   */
  void SetAnyValueItemContainer(mvvm::SessionItem* container);

  void SetJSONPretty(bool value);

signals:
  void ExportToFileRequest();

private:
  void SetupActions();
  void UpdateJson();
  void SetupListener();
  AnyValueItem* GetAnyValueItem();

  QAction* m_export_action{nullptr};
  QAction* m_pretty_action{nullptr};

  CodeView* m_json_view{nullptr};
  mvvm::ISessionModel* m_model{nullptr};
  mvvm::SessionItem* m_container{nullptr};
  std::unique_ptr<listener_t> m_listener;
  bool m_pretty_json{true};
  sup::gui::VisibilityAgentBase* m_visibility_agent{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_TEXTPANEL_H_
