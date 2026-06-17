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

#include <sup/gui/component_objects/tab_widget_controller.h>

namespace sup::gui
{

ConfigurationPanelMirrorController::ConfigurationPanelMirrorController(QObject* parent_object)
    : QObject(parent_object)
    , m_tab_widget_controller(std::make_unique<TabWidgetController>())
{
}

ConfigurationPanelMirrorController::~ConfigurationPanelMirrorController() = default;

void ConfigurationPanelMirrorController::ProcessPanelAdd(sup::gui::SplittableComboPanel* panel)
{
  // The registration of the panel's synchronisable widgets in the underlying controllers will be
  // implemented in the next step.
  (void)panel;
}

void ConfigurationPanelMirrorController::ProcessPanelRemove(sup::gui::SplittableComboPanel* panel)
{
  // The unregistration of the panel's synchronisable widgets from the underlying controllers will
  // be implemented in the next step.
  (void)panel;
}

TabWidgetController* ConfigurationPanelMirrorController::GetTabWidgetController() const
{
  return m_tab_widget_controller.get();
}

}  // namespace sup::gui
