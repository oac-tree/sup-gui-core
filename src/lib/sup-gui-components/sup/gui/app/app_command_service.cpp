/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "app_command_service.h"

#include <sup/gui/app/app_command_context.h>
#include <sup/gui/app/i_action_container.h>
#include <sup/gui/app/i_action_container_registry.h>
#include <sup/gui/app/i_app_command_context_widget_map.h>
#include <sup/gui/app/i_app_command_registry.h>

namespace sup::gui
{

AppCommandService::AppCommandService(
    std::unique_ptr<IAppCommandRegistry> command_registry,
    std::unique_ptr<IAppCommandContextWidgetMap> context_widget_map,
    std::unique_ptr<IActionContainerRegistry> container_registry)
    : m_command_registry(std::move(command_registry))
    , m_command_context_widget_map(std::move(context_widget_map))
    , m_action_container_registry(std::move(container_registry))

{
}

AppCommandService::~AppCommandService() = default;

AppCommandContext AppCommandService::RegisterWidgetUniqueId(const QWidget *widget)
{
  return m_command_context_widget_map->RegisterWidgetUniqueId(widget);
}

void AppCommandService::UnregisterWidgetUniqueId(const QWidget *widget)
{
  m_command_context_widget_map->UnregisterWidgetUniqueId(widget);
}

}  // namespace sup::gui
