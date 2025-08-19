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

#include "app_command.h"
#include "app_context_focus_controller.h"

#include <sup/gui/app/app_command_context.h>
#include <sup/gui/app/i_action_container.h>
#include <sup/gui/app/i_action_container_registry.h>
#include <sup/gui/app/i_app_command_context_widget_map.h>
#include <sup/gui/app/i_app_command_registry.h>
#include <sup/gui/components/proxy_action.h>
#include <sup/gui/core/sup_gui_core_exceptions.h>

namespace sup::gui
{

AppCommandService::AppCommandService(
    std::unique_ptr<IAppCommandRegistry> command_registry,
    std::unique_ptr<IAppCommandContextWidgetMap> context_widget_map,
    std::unique_ptr<IActionContainerRegistry> menubar_action_registry)
    : m_command_registry(std::move(command_registry))
    , m_command_context_widget_map(std::move(context_widget_map))
    , m_app_menubar_action_registry(std::move(menubar_action_registry))
    , m_focus_controller(std::make_unique<AppContextFocusController>(*m_command_context_widget_map,
                                                                     *m_command_registry))
{
}

AppCommandService::~AppCommandService() = default;

void AppCommandService::AppRegisterMenuBar(QMenuBar *menubar, const QStringList &top_menu_names)
{
  m_app_menubar_action_registry->SetMenuBar(menubar);
  for (const auto &name : top_menu_names)
  {
    (void)m_app_menubar_action_registry->AddMenu(name);
  }
}

AppCommandContext AppCommandService::RegisterWidgetUniqueId(const QWidget *widget)
{
  return m_command_context_widget_map->RegisterWidgetUniqueId(widget);
}

void AppCommandService::UnregisterWidgetUniqueId(const QWidget *widget)
{
  m_command_context_widget_map->UnregisterWidgetUniqueId(widget);
}

QMenu *AppCommandService::AppGetMenu(const QString &menu_name)
{
  auto container = m_app_menubar_action_registry->GetContainer(menu_name);
  return container ? container->GetMenu() : nullptr;
}

bool AppCommandService::AddActionToMenuBar(const QString &menu_name, QAction *action)
{
  return m_app_menubar_action_registry->RegisterAction(menu_name, action);
}

AppCommand *AppCommandService::AddCommandToMenu(const QString &menu_name, const QString &command_id)
{
  // register new, or get access to already registered command
  auto command = m_command_registry->RegisterCommand(command_id, command_id);

  // add underlying proxy action to the menu
  if (!AddActionToMenuBar(menu_name, command->GetProxyAction()))
  {
    throw RuntimeException("Global menu [" + menu_name.toStdString() + "] doesn't exist");
  }
  return command;
}

bool AppCommandService::AddActionToCommand(QAction *action, const QString &command_id,
                                           const AppCommandContext &context)
{
  auto app_command = m_command_registry->RegisterAction(action, command_id, context);
  return app_command != nullptr;
}

AppCommand *AppCommandService::GetCommand(const QString &command_id)
{
  return m_command_registry->GetCommand(command_id);
}

void AppCommandService::OnFocusWidgetUpdate(QWidget *old, QWidget *now)
{
  m_focus_controller->OnFocusWidgetUpdate(old, now);
}

}  // namespace sup::gui
