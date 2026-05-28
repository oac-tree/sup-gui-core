/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_CONFIG_TAB_WIDGET_H_
#define SUP_GUI_VIEWS_DTOEDITOR_CONFIG_TAB_WIDGET_H_

#include <QWidget>

class QTabWidget;
class QToolBar;

namespace sup::gui
{

class AnyValueEditorTreePanel;
class AnyValueEditorTextPanel;
class ConfigurationItem;

/**
 * @brief The ConfigTabWidget class is a configuration editor with multiple different views of
 * the same data.
 *
 * Views are placed in QTabWidget. For the moment contains
 * - AnyValueEditorTreePanel
 * - AnyValueEditorTextPanel
 */
class ConfigTabWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ConfigTabWidget(QWidget* parent_widget = nullptr);

  /**
   * @brief Sets the configuration to be edited.
   */
  void SetConfiguration(ConfigurationItem* configuration_item);

private:
  void UpdateToolbarButtons();
  void AddTab(QWidget* widget, const QString& label, const QString& tooltip);

  QToolBar* m_tool_bar{nullptr};
  AnyValueEditorTreePanel* m_tree_panel{nullptr};
  AnyValueEditorTextPanel* m_text_panel{nullptr};
  QTabWidget* m_tab_widget{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_CONFIG_TAB_WIDGET_H_
