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

#include "dto_explorer_panel.h"

#include "configuration_list_widget.h"

#include <sup/gui/widgets/collapsible_list_view.h>
#include <sup/gui/widgets/file_tree_view.h>
#include <sup/gui/widgets/item_stack_widget.h>

#include <QFileInfo>
#include <QVBoxLayout>

namespace sup::gui
{

namespace
{
constexpr auto kFileTreeSettingsGroup = "FileTreeView";
constexpr auto kCollapsibleListSettingName = "DtoExplorerPanel/collapsible_list";

/**
 * @brief Checks if given file is AnyValue JSON file candidate.
 */
bool IsJSONFile(const QString& file_info)
{
  const QFileInfo info(file_info);
  return info.isFile() && info.completeSuffix().toLower() == QStringLiteral("json");
}

}  // namespace

DtoExplorerPanel::DtoExplorerPanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_collapsible_list(new sup::gui::CollapsibleListView(kCollapsibleListSettingName))
    , m_file_tree_view(new sup::gui::FileTreeView(kFileTreeSettingsGroup))
    , m_configuration_list_widget(new ConfigurationListWidget)
    , m_stack_widget(new sup::gui::ItemStackWidget)
{
  m_collapsible_list->setWindowTitle("Explorer View");

  m_collapsible_list->AddWidget(m_file_tree_view);
  m_collapsible_list->AddCollapsibleWidget(m_configuration_list_widget, {});

  // m_file_tree_view->setWindowTitle("Explorer View");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_stack_widget->AddWidget(m_collapsible_list, m_file_tree_view->actions());
  layout->addWidget(m_stack_widget);

  ReadSettings();

  connect(m_file_tree_view, &FileTreeView::FileClicked, this, &DtoExplorerPanel::FileClicked);

  auto on_file_double_clicked = [this](const QString& file_name)
  {
    if (IsJSONFile(file_name))
    {
      emit FileDoubleClicked(file_name);
    }
  };

  connect(m_file_tree_view, &FileTreeView::FileDoubleClicked, this, on_file_double_clicked);
  connect(m_configuration_list_widget, &ConfigurationListWidget::SelectionChanged, this,
          &DtoExplorerPanel::SelectionChanged);
}

DtoExplorerPanel::~DtoExplorerPanel()
{
  WriteSettings();
}

void DtoExplorerPanel::SetConfigurationContainer(ConfigurationContainerItem* container)
{
  m_configuration_list_widget->SetConfigurationContainer(container);
}

void DtoExplorerPanel::ReadSettings()
{
  m_collapsible_list->ReadSettings();
}

void DtoExplorerPanel::WriteSettings()
{
  m_collapsible_list->WriteSettings();
}

}  // namespace sup::gui
