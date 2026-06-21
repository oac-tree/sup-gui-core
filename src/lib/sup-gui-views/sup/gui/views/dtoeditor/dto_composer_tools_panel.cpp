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

#include "dto_composer_tools_panel.h"

#include "configuration_list_widget.h"
#include "dto_compare_tool_bar.h"

#include <mvvm/model/item_selection.h>

#include <QVBoxLayout>

namespace sup::gui
{

DtoComposerToolsPanel::DtoComposerToolsPanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tool_bar(new DtoCompareToolBar)
    , m_configuration_list_widget(new ConfigurationListWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_tool_bar);
  layout->addWidget(m_configuration_list_widget);

  connect(m_tool_bar, &DtoCompareToolBar::panelSyncAppearanceFlagChanged, this,
          &DtoComposerToolsPanel::panelSyncAppearanceFlagChanged);
  connect(m_configuration_list_widget, &ConfigurationListWidget::SelectionChanged, this,
          &DtoComposerToolsPanel::SelectionChanged);
}

void DtoComposerToolsPanel::SetConfigurationContainer(ConfigurationContainerItem* container)
{
  m_configuration_list_widget->SetConfigurationContainer(container);
}

mvvm::ItemSelection DtoComposerToolsPanel::GetSelection() const
{
  return m_configuration_list_widget->GetSelection();
}

void DtoComposerToolsPanel::SetSelection(const mvvm::ItemSelection& selection)
{
  m_configuration_list_widget->SetSelection(selection);
}

bool DtoComposerToolsPanel::GetPanelSyncAppearanceFlag() const
{
  return m_tool_bar->GetPanelSyncAppearanceFlag();
}

}  // namespace sup::gui
