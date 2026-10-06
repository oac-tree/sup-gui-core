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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_CONFIGURATION_TAB_WIDGET_H_
#define SUP_GUI_VIEWS_DTOEDITOR_CONFIGURATION_TAB_WIDGET_H_

#include <sup/gui/components/component_types.h>
#include <sup/gui/mainwindow/session_item_widget.h>

class QTabWidget;
class QAbstractScrollArea;
class QTreeView;

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{

class AnyValueEditorTreePanel;
class AnyValueEditorTextPanel;
class ISettingsManager;

/**
 * @brief The ConfigurationTabWidget class is a configuration editor with multiple different views
 * of the same data.
 *
 * Views are placed in QTabWidget. For the moment contains
 * - AnyValueEditorTreePanel
 * - AnyValueEditorTextPanel
 */
class ConfigurationTabWidget : public SessionItemWidget
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * The settings manager must outlive the widget.
   */
  explicit ConfigurationTabWidget(ISettingsManager& settings_manager,
                                  QWidget* parent_widget = nullptr);

  /**
   * @brief Sets the configuration to be edited.
   */
  void SetItem(mvvm::SessionItem* configuration_item) override;

  /**
   * @brief Returns configuration server by this widget.
   */
  const mvvm::SessionItem* GetItem() const override;

  /**
   * @brief Returns the tab widget holding the different views of the configuration.
   *
   * Used to provide appearance synchronisation between QTabWidget belonging to different panels.
   */
  QTabWidget* GetTabWidget() const;

  /**
   * @brief Returns main scroll area of the view corresponding to the given mode.
   *
   * Used to couple scroll positions of different views of different configuration panels.
   */
  QAbstractScrollArea* GetScrollArea(ConfigurationViewMode mode) const;

  /**
   * @brief Returns the tree view of the configuration tree panel.
   */
  QTreeView* GetMainTreeView() const;

private:
  void AddTab(QWidget* widget, const QString& label, const QString& tooltip);
  void OnContextMenuRequest(const QPoint& point);

  AnyValueEditorTreePanel* m_tree_panel{nullptr};
  AnyValueEditorTextPanel* m_text_panel{nullptr};
  QTabWidget* m_tab_widget{nullptr};
  const mvvm::SessionItem* m_configuration_item{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_CONFIGURATION_TAB_WIDGET_H_
