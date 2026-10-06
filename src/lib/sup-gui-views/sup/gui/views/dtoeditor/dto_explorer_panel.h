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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_EXPLORER_PANEL_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_EXPLORER_PANEL_H_

#include <QWidget>

namespace mvvm
{
class ItemSelection;
}  // namespace mvvm

namespace sup::gui
{

class FileTreeView;
class ItemStackWidget;
class CollapsibleListView;
class ConfigurationListWidget;
class ConfigurationContainerItem;
class ISettingsManager;

class DtoExplorerPanel : public QWidget
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * The settings manager must outlive the panel.
   */
  explicit DtoExplorerPanel(ISettingsManager& settings_manager, QWidget* parent_widget = nullptr);

  ~DtoExplorerPanel() override;

  DtoExplorerPanel(const DtoExplorerPanel&) = delete;
  DtoExplorerPanel& operator=(const DtoExplorerPanel&) = delete;
  DtoExplorerPanel(DtoExplorerPanel&&) noexcept = delete;
  DtoExplorerPanel& operator=(DtoExplorerPanel&&) noexcept = delete;

  void SetConfigurationContainer(ConfigurationContainerItem* container);

  mvvm::ItemSelection GetSelection() const;

  void SetSelection(const mvvm::ItemSelection& selection);

signals:
  void FileClicked(const QString& filename);
  void FileDoubleClicked(const QString& filename);
  void SelectionChanged(const mvvm::ItemSelection&);

private:
  sup::gui::CollapsibleListView* m_collapsible_list{nullptr};
  FileTreeView* m_file_tree_view;
  ConfigurationListWidget* m_configuration_list_widget{nullptr};
  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_EXPLORER_PANEL_H_
