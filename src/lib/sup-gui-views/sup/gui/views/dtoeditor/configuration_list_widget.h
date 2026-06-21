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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_CONFIGURATION_LIST_WIDGET_H_
#define SUP_GUI_VIEWS_DTOEDITOR_CONFIGURATION_LIST_WIDGET_H_

#include <QWidget>

namespace mvvm
{
class ItemViewComponentProvider;
class ItemSelection;
}  // namespace mvvm

namespace sup::gui
{

class ConfigurationItem;
class ConfigurationContainerItem;
class FlatListView;

/**
 * @brief The ConfigurationListWidget class shows the list of all opened configurations.
 *
 * Contains FlatListView with the possibility to close opened configurations.
 */
class ConfigurationListWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ConfigurationListWidget(QWidget* parent_widget = nullptr);

  ~ConfigurationListWidget() override;

  ConfigurationListWidget(const ConfigurationListWidget&) = delete;
  ConfigurationListWidget& operator=(const ConfigurationListWidget&) = delete;
  ConfigurationListWidget(ConfigurationListWidget&&) noexcept = delete;
  ConfigurationListWidget& operator=(ConfigurationListWidget&&) noexcept = delete;

  void SetConfigurationContainer(ConfigurationContainerItem* container);

  mvvm::ItemSelection GetSelection() const;

  void SetSelection(const mvvm::ItemSelection& selection);

signals:
  void SelectionChanged(const mvvm::ItemSelection& selection);

private:
  std::unique_ptr<mvvm::ItemViewComponentProvider> CreateProvider() const;
  void SetupConnections();
  void SetupActions();
  void OnAddConfiguration();
  void OnRemoveConfiguration();

  FlatListView* m_list_view{nullptr};
  QAction* m_add_configuration_action{nullptr};
  QAction* m_remove_configuration_action{nullptr};
  ConfigurationContainerItem* m_configuration_container{nullptr};

  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_CONFIGURATION_LIST_WIDGET_H_
