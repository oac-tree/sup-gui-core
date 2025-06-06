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

#ifndef SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_TEXTPANEL_H_
#define SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_TEXTPANEL_H_

#include <QWidget>

class QAction;
class QToolButton;

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{

class CodeView;
class VisibilityAgentBase;
class AnyValueItem;
class IMessageHandler;
class JsonPanelController;

/**
 * @brief The AnyValueEditorTextPanel class represents a collapsible panel on the right of
 * AnyValueEditor with JSON representation of AnyValue.
 */
class AnyValueEditorTextPanel : public QWidget
{
  Q_OBJECT

public:
  explicit AnyValueEditorTextPanel(QWidget* parent_widget = nullptr);
  ~AnyValueEditorTextPanel() override;

  /**
   * @brief Sets the container with AnyValueItem.
   */
  void SetAnyValueItemContainer(mvvm::SessionItem* container);

  void SetJsonPretty(bool value);

signals:
  void ExportToFileRequest();

private:
  void SetContainerIntern(mvvm::SessionItem* container);
  void SendMessage(const std::string& what) const;
  void SetupActions();

  QAction* m_export_action{nullptr};
  QAction* m_pretty_action{nullptr};

  CodeView* m_json_view{nullptr};
  mvvm::SessionItem* m_container{nullptr};
  VisibilityAgentBase* m_visibility_agent{nullptr};
  std::unique_ptr<IMessageHandler> m_message_handler;
  std::unique_ptr<JsonPanelController> m_panel_controller;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_ANYVALUEEDITOR_ANYVALUE_EDITOR_TEXTPANEL_H_
