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
constexpr auto kPlaceholderText = "Select one of available configurations";
}

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

  // SetPlaceholderMode(PlaceholderMode::kOn);
  // SetupConnections();

}

void ConfigComboPanel::SetModel(ConfigurationModel* model)
{
  m_model = model;
}

void ConfigComboPanel::SetConfiguration(ConfigurationItem* configuration_item) {}

}  // namespace sup::gui
