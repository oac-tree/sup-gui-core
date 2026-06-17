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

#include "configuration_panel_mirror_controller.h"

#include "configuration_tab_widget.h"
#include "splittable_combo_panel.h"

#include <sup/gui/component_objects/tab_widget_controller.h>

namespace
{

//! Returns the QTabWidget of the panel's main editor, or nullptr if not available.
QTabWidget* GetPanelTabWidget(sup::gui::SplittableComboPanel* panel)
{
  if (panel == nullptr)
  {
    return nullptr;
  }

  auto* editor = panel->GetMainEditor<sup::gui::ConfigurationTabWidget>();
  return editor == nullptr ? nullptr : editor->GetTabWidget();
}

}  // namespace

namespace sup::gui
{

ConfigurationPanelMirrorController::ConfigurationPanelMirrorController(QObject* parent_object)
    : QObject(parent_object), m_tab_widget_controller(std::make_unique<TabWidgetController>())
{
}

ConfigurationPanelMirrorController::~ConfigurationPanelMirrorController() = default;

void ConfigurationPanelMirrorController::SetActive(bool value)
{
  m_tab_widget_controller->SetActive(value);
}

void ConfigurationPanelMirrorController::ProcessPanelAdd(sup::gui::SplittableComboPanel* panel)
{
  if (auto* tab_widget = GetPanelTabWidget(panel); tab_widget != nullptr)
  {
    m_tab_widget_controller->AddTabWidget(tab_widget);
  }
}

void ConfigurationPanelMirrorController::ProcessPanelRemove(sup::gui::SplittableComboPanel* panel)
{
  if (auto* tab_widget = GetPanelTabWidget(panel); tab_widget != nullptr)
  {
    m_tab_widget_controller->RemoveTabWidget(tab_widget);
  }
}

TabWidgetController* ConfigurationPanelMirrorController::GetTabWidgetController() const
{
  return m_tab_widget_controller.get();
}

}  // namespace sup::gui
