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
#include <sup/gui/component_objects/tree_expand_controller.h>
#include <sup/gui/component_objects/tree_highlight_controller.h>

#include <QTreeView>

namespace
{

//! Returns the configuration editor of the panel's main editor, or nullptr if not available.
sup::gui::ConfigurationTabWidget* GetPanelEditor(sup::gui::SplittableComboPanel* panel)
{
  return panel == nullptr ? nullptr : panel->GetMainEditor<sup::gui::ConfigurationTabWidget>();
}

}  // namespace

namespace sup::gui
{

ConfigurationPanelMirrorController::ConfigurationPanelMirrorController(QObject* parent_object)
    : QObject(parent_object)
    , m_tab_widget_controller(std::make_unique<TabWidgetController>())
    , m_json_area_controller(std::make_unique<ScrollAreaMirrorController>())
    , m_tree_area_controller(std::make_unique<ScrollAreaMirrorController>())
    , m_tree_expand_controller(std::make_unique<TreeExpandController>())
    , m_tree_highlight_controller(std::make_unique<TreeHighlightController>())
{
}

ConfigurationPanelMirrorController::~ConfigurationPanelMirrorController() = default;

void ConfigurationPanelMirrorController::SetActive(bool value)
{
  m_tab_widget_controller->SetActive(value);
  m_json_area_controller->SetActive(value);
  m_tree_area_controller->SetActive(value);
  m_tree_expand_controller->SetActive(value);
  m_tree_highlight_controller->SetActive(value);
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

  m_tree_expand_controller->AddTreeView(editor->GetMainTreeView());

  m_tree_highlight_controller->AddViewModel(editor->GetMainTreeView()->model());
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

  m_tree_expand_controller->RemoveTreeView(editor->GetMainTreeView());

  m_tree_highlight_controller->RemoveViewModel(editor->GetMainTreeView()->model());
}

void ConfigurationPanelMirrorController::SetReferencePanel(sup::gui::SplittableComboPanel* panel)
{
  auto* editor = GetPanelEditor(panel);
  auto* reference_model = editor == nullptr ? nullptr : editor->GetMainTreeView()->model();
  m_tree_highlight_controller->SetReferenceViewModel(reference_model);
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

TreeExpandController* ConfigurationPanelMirrorController::GetTreeExpandController() const
{
  return m_tree_expand_controller.get();
}

TreeHighlightController* ConfigurationPanelMirrorController::GetTreeHighlightController() const
{
  return m_tree_highlight_controller.get();
}

}  // namespace sup::gui
