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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPARE_TOOLS_PANEL_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPARE_TOOLS_PANEL_H_

#include <QWidget>

namespace mvvm
{
class ItemSelection;
}  // namespace mvvm

namespace sup::gui
{

class ConfigurationListWidget;
class ConfigurationContainerItem;
class DtoCompareToolBar;

/**
 * @brief The DtoCompareToolsPanel class is a left panel of DtoCompareView.
 *
 * Contains a toolbar and a tree with opened configurations.
 */
class DtoCompareToolsPanel : public QWidget
{
  Q_OBJECT

public:
  explicit DtoCompareToolsPanel(QWidget* parent_widget = nullptr);

  void SetConfigurationContainer(ConfigurationContainerItem* container);

  mvvm::ItemSelection GetSelection() const;

  void SetSelection(const mvvm::ItemSelection& selection);

signals:
  void SelectionChanged(const mvvm::ItemSelection&);
  void panelSyncAppearanceFlagChanged(bool value);

private:
  DtoCompareToolBar* m_tool_bar{nullptr};
  ConfigurationListWidget* m_configuration_list_widget{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_COMPARE_TOOLS_PANEL_H_
