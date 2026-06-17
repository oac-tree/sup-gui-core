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

#include <sup/gui/component_objects/scroll_area_mirror_controller.h>
#include <sup/gui/component_objects/tab_widget_controller.h>

namespace
{

//! Returns the configuration editor of the panel's main editor, or nullptr if not available.
sup::gui::ConfigurationTabWidget* GetPanelEditor(sup::gui::SplittableComboPanel* panel)
{
  return panel == nullptr ? nullptr
                          : panel->GetMainEditor<sup::gui::ConfigurationTabWidget>();
}

}  // namespace

namespace sup::gui
{

ConfigurationPanelMirrorController::ConfigurationPanelMirrorController(QObject* parent_object)
    : QObject(parent_object)
    , m_tab_widget_controller(std::make_unique<TabWidgetController>())
    , m_json_area_controller(std::make_unique<ScrollAreaMirrorController>())
    , m_tree_area_controller(std::make_unique<ScrollAreaMirrorController>())
{
}

ConfigurationPanelMirrorController::~ConfigurationPanelMirrorController() = default;

void ConfigurationPanelMirrorController::SetActive(bool value)
{
  m_tab_widget_controller->SetActive(value);
  m_json_area_controller->SetActive(value);
  m_tree_area_controller->SetActive(value);
}

void ConfigurationPanelMirrorController::ProcessPanelAdd(sup::gui::SplittableComboPanel* panel)
{
  auto* editor = GetPanelEditor(panel);
  if (editor == nullptr)
  {
    return;
  }

  m_tab_widget_controller->AddTabWidget(editor->GetTabWidget());

  if (auto* json_area = editor->GetScrollArea(ConfigurationViewMode::kJSON); json_area != nullptr)
  {
    m_json_area_controller->AddScrollArea(json_area);
  }

  if (auto* tree_area = editor->GetScrollArea(ConfigurationViewMode::kTree); tree_area != nullptr)
  {
    m_tree_area_controller->AddScrollArea(tree_area);
  }
}

void ConfigurationPanelMirrorController::ProcessPanelRemove(sup::gui::SplittableComboPanel* panel)
{
  auto* editor = GetPanelEditor(panel);
  if (editor == nullptr)
  {
    return;
  }

  m_tab_widget_controller->RemoveTabWidget(editor->GetTabWidget());

  if (auto* json_area = editor->GetScrollArea(ConfigurationViewMode::kJSON); json_area != nullptr)
  {
    m_json_area_controller->RemoveScrollArea(json_area);
  }

  if (auto* tree_area = editor->GetScrollArea(ConfigurationViewMode::kTree); tree_area != nullptr)
  {
    m_tree_area_controller->RemoveScrollArea(tree_area);
  }
}

TabWidgetController* ConfigurationPanelMirrorController::GetTabWidgetController() const
{
  return m_tab_widget_controller.get();
}

ScrollAreaMirrorController* ConfigurationPanelMirrorController::GetJsonAreaController() const
{
  return m_json_area_controller.get();
}

ScrollAreaMirrorController* ConfigurationPanelMirrorController::GetTreeAreaController() const
{
  return m_tree_area_controller.get();
}

}  // namespace sup::gui
