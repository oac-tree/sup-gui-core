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

#include "config_combo_panel.h"

#include "config_combo_adapter.h"
#include "config_tab_widget.h"

#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/widgets/combo_tool_bar.h>
#include <sup/gui/widgets/placeholder_widget.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/signals/model_listener.h>

#include <QStackedWidget>
#include <QVBoxLayout>

namespace sup::gui
{

namespace
{
constexpr auto kPlaceholderText = "Select one of available configurations";
}  // namespace

ConfigComboPanel::ConfigComboPanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tool_bar(new ComboToolBar(CreateConfigurationNamesCallback()))
    , m_stacked_widget(new QStackedWidget)
    , m_placeholder_widget(new PlaceholderWidget(kPlaceholderText))
    , m_config_widget(new ConfigTabWidget())
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_tool_bar);
  layout->addWidget(m_stacked_widget);

  m_stacked_widget->addWidget(m_placeholder_widget);
  m_stacked_widget->addWidget(m_config_widget);

  SetPlaceholderMode(PlaceholderMode::kShowPlaceholder);
  SetupConnections();
}

ConfigComboPanel::~ConfigComboPanel() = default;

ConfigurationModel* ConfigComboPanel::GetModel()
{
  return m_model;
}

void ConfigComboPanel::SetModel(ConfigurationModel* model)
{
  m_model = model;
  if (m_model != nullptr)
  {
    SetupModelListener();
  }
  else
  {
    m_listener.reset();
  }

  m_tool_bar->SetCurrentIndex(0);
}

void ConfigComboPanel::SetConfiguration(ConfigurationItem* configuration_item)
{
  ConfigComboAdapter adapter(GetAvailableConfigurations());
  m_tool_bar->SetCurrentIndex(adapter.GetComboIndexFromItem(configuration_item));

  // the combo index change will trigger the ShowConfigurationInEditor method, so no need to call it
  // here explicitly
}

ConfigurationItem* ConfigComboPanel::GetCurrentConfiguration()
{
  return m_current_configuration_item;
}

std::vector<ConfigurationItem*> ConfigComboPanel::GetAvailableConfigurations() const
{
  if ((m_model == nullptr) || !m_model->GetConfigurationContainer())
  {
    return {};
  }

  return m_model->GetConfigurationContainer()->GetConfigurations();
}

void ConfigComboPanel::SetPlaceholderMode(PlaceholderMode mode)
{
  m_stacked_widget->setCurrentIndex(static_cast<std::int32_t>(mode));
}

void ConfigComboPanel::SetupConnections()
{
  connect(m_tool_bar, &ComboToolBar::splitViewRequest, this, &ConfigComboPanel::splitViewRequest);
  connect(m_tool_bar, &ComboToolBar::closeViewRequest, this, &ConfigComboPanel::closeViewRequest);

  auto on_combo_index_changed = [this](auto index)
  {
    ConfigComboAdapter adapter(GetAvailableConfigurations());
    ShowConfigurationInEditor(adapter.GetItemFromComboIndex(index));
  };
  connect(m_tool_bar, &ComboToolBar::selectItemRequest, this, on_combo_index_changed);
}

void ConfigComboPanel::SetupModelListener()
{
  m_listener = std::make_unique<mvvm::ModelListener>(m_model);

  auto on_data_changed = [this](const mvvm::DataChangedEvent& event)
  {
    if (event.item == m_current_configuration_item)
    {
      // assume that the display name might be changed and update the combo box text
      m_tool_bar->UpdateAppearance();
    }
  };
  m_listener->Connect<mvvm::DataChangedEvent>(on_data_changed);

  auto on_about_to_remove_item = [this](const mvvm::AboutToRemoveItemEvent& event)
  {
    auto item_to_remove = event.item->GetItem(event.tag_index);

    if ((item_to_remove == m_current_configuration_item)
        || mvvm::utils::IsItemAncestor(m_current_configuration_item, item_to_remove))
    {
      SetConfiguration(nullptr);
    }
  };
  m_listener->Connect<mvvm::AboutToRemoveItemEvent>(on_about_to_remove_item);
}

void ConfigComboPanel::ShowConfigurationInEditor(ConfigurationItem* item)
{
  if (m_current_configuration_item == item)
  {
    return;
  }

  m_current_configuration_item = item;

  m_config_widget->SetConfiguration(m_current_configuration_item);
  SetPlaceholderMode(m_current_configuration_item == nullptr ? PlaceholderMode::kShowPlaceholder
                                                             : PlaceholderMode::kShowEditor);
  emit selectedConfigurationChanged(mvvm::ItemSelection(m_current_configuration_item));
}

std::function<QList<QString>()> ConfigComboPanel::CreateConfigurationNamesCallback()
{
  return [this]()
  {
    const ConfigComboAdapter adapter(GetAvailableConfigurations());
    return adapter.GetAvailableItemNames();
  };
}

}  // namespace sup::gui
