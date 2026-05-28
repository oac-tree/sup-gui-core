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

#include "config_tab_widget.h"

#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/widgets/combo_tool_bar.h>
#include <sup/gui/widgets/placeholder_widget.h>

#include <QStackedWidget>
#include <QVBoxLayout>

namespace sup::gui
{

namespace
{
constexpr auto kNoSelection = " < no selection >";
constexpr auto kPlaceholderText = "Select one of available configurations";
}  // namespace

ConfigComboPanel::ConfigComboPanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tool_bar(new ComboToolBar({}))
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

void ConfigComboPanel::SetModel(ConfigurationModel* model)
{
  m_model = model;
}

void ConfigComboPanel::SetConfiguration(ConfigurationItem* configuration_item)
{
  // m_tool_bar->UpdateProcedureSelectionMenu(procedure_item);
  // SetProcedureIntern(procedure_item);
}

QList<QString> ConfigComboPanel::GetAvailableConfigurationNames() const
{
  QList<QString> result{kNoSelection};
  const auto configurations = GetAvailableConfigurations();
  for (const auto& item : configurations)
  {
    result.append(QString::fromStdString(item->GetDisplayName()));
  }
  return result;
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
    auto item = GetConfigurationFromComboIndex(index);
    ShowConfigurationInEditor(item);
  };

  connect(m_tool_bar, &ComboToolBar::selectItemRequest, this, on_combo_index_changed);
}

void ConfigComboPanel::ShowConfigurationInEditor(ConfigurationItem* item)
{
  if (m_current_configuration_item == item)
  {
    return;
  }

  m_current_configuration_item = item;

  m_config_widget->SetConfiguration(m_current_configuration_item);
  SetPlaceholderMode(item == nullptr ? PlaceholderMode::kShowPlaceholder
                                     : PlaceholderMode::kShowEditor);
  emit selectedConfigurationChanged(m_current_configuration_item);
}

ConfigurationItem* ConfigComboPanel::GetConfigurationFromComboIndex(
    std::int32_t combo_index) const
{
  // first index is always "no selection"
  const std::int32_t index = combo_index - 1;
  const auto configurations = GetAvailableConfigurations();
  if (index < 0 || index >= static_cast<std::int32_t>(configurations.size()))
  {
    return nullptr;
  }

  return configurations.at(index);
}

std::int32_t ConfigComboPanel::GetComboIndexFromConfiguration(const ConfigurationItem* item)
{
  // first index is always "no selection"
  if (item == nullptr)
  {
    return 0;
  }

  return static_cast<std::int32_t>(item->GetTagIndex().GetIndex() + 1);
}

}  // namespace sup::gui
