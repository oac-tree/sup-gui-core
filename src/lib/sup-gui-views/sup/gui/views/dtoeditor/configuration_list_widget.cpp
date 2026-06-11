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

#include "configuration_list_widget.h"

#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/widgets/flatlist_delegate.h>
#include <sup/gui/widgets/flatlist_view.h>

#include <mvvm/model/item_selection.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/viewmodel/top_items_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace sup::gui
{

ConfigurationListWidget::ConfigurationListWidget(QWidget* parent_widget)
    : QWidget(parent_widget), m_list_view(new FlatListView), m_component_provider(CreateProvider())

{
  setWindowTitle("CONFIGURATIONS");
  setToolTip("List of currently opened configurations");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_list_view);

  SetupConnections();
}

ConfigurationListWidget::~ConfigurationListWidget() = default;

void ConfigurationListWidget::SetConfigurationContainer(ConfigurationContainerItem* container)
{
  m_component_provider->SetItem(container);
}

const ConfigurationItem* ConfigurationListWidget::GetSelectedConfiguration() const
{
  auto selected = GetSelectedConfigurations();
  return selected.empty() ? nullptr : selected.front();
}

std::vector<const ConfigurationItem*> ConfigurationListWidget::GetSelectedConfigurations() const
{
  auto selection = m_component_provider->GetSelection();
  return selection.GetSelectedItems<ConfigurationItem>();
}

void ConfigurationListWidget::SetSelectedConfiguration(const ConfigurationItem* config)
{
  m_component_provider->SetSelection(mvvm::ItemSelection(config));
}

std::unique_ptr<mvvm::ItemViewComponentProvider> ConfigurationListWidget::CreateProvider() const
{
  auto viewmodel = std::make_unique<mvvm::TopItemsViewModel>(nullptr);
  auto delegate = std::make_unique<sup::gui::FlatListDelegate>();

  return std::make_unique<mvvm::ItemViewComponentProvider>(std::move(delegate),
                                                           std::move(viewmodel), m_list_view);
}

void ConfigurationListWidget::SetupConnections()
{
  // handles click on close button in column=1
  auto on_close = [this](const QModelIndex& index)
  {
    auto configuration_item = m_component_provider->GetItemFromViewIndex(index);
    if (configuration_item != nullptr)
    {
      mvvm::utils::RemoveItem(*configuration_item);
    }
  };

  connect(m_list_view, &sup::gui::FlatListView::closeHandleClicked, this, on_close);

  connect(m_component_provider.get(), &mvvm::ItemViewComponentProvider::SelectionChanged, this,
          &ConfigurationListWidget::SelectionChanged);
}

}  // namespace sup::gui
