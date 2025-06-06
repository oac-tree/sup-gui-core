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

#include "app_action_helper.h"

#include "app_action_manager.h"
#include "app_command.h"
#include "app_command_manager.h"
#include "app_context_focus_controller.h"
#include "app_context_manager.h"

#include <sup/gui/components/proxy_action.h>
#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <QApplication>
#include <QWidget>

namespace sup::gui
{

ActionManager &GetGlobalActionManager()
{
  static ActionManager global_action_manager;
  return global_action_manager;
}

AppContextManager &GetGlobalContextManager()
{
  static AppContextManager global_context_manager;
  return global_context_manager;
}

AppCommandManager &GetGlobalCommandManager()
{
  static AppCommandManager global_command_manager;
  return global_command_manager;
}

std::unique_ptr<AppContextFocusController> CreateGlobalFocusController()
{
  auto result = std::make_unique<AppContextFocusController>(GetGlobalContextManager(),
                                                            GetGlobalCommandManager(), nullptr);
  (void)QObject::connect(qApp, &QApplication::focusChanged, result.get(),
                         &sup::gui::AppContextFocusController::OnFocusWidgetUpdate);
  return result;
}

void AppRegisterMenuBar(QMenuBar *menubar)
{
  GetGlobalActionManager().SetMenuBar(menubar);
}

void AppRegisterMenuBar(QMenuBar *menubar, const QStringList &names)
{
  AppRegisterMenuBar(menubar);
  for (const auto &name : names)
  {
    (void)AppAddMenu(name);
  }
}

IActionContainer *AppAddMenu(const QString &menu_name)
{
  return GetGlobalActionManager().AddMenu(menu_name);
}

QMenu *AppGetMenu(const QString &menu_name)
{
  auto container = GetGlobalActionManager().GetContainer(menu_name);
  return container ? container->GetMenu() : nullptr;
}

bool AppAddActionToMenuBar(const QString &menu_name, QAction *action)
{
  return GetGlobalActionManager().RegisterAction(menu_name, action);
}

bool AppAddActionsToMenuBar(const QString &menu_name, const QList<QAction *> &actions)
{
  bool result = true;
  for (auto action : actions)
  {
    result &= AppAddActionToMenuBar(menu_name, action);
  }
  return result;
}

AppContext AppRegisterWidgetUniqueId(const QWidget *widget)
{
  return GetGlobalContextManager().RegisterWidgetUniqueId(widget);
}

void AppUnregisterWidgetUniqueId(const QWidget *widget)
{
  GetGlobalContextManager().UnregisterWidgetUniqueId(widget);
}

AppCommand *AppAddCommandToMenu(const QString &menu_name, const QString &command_id)
{
  // register new, or get access to already registered command
  auto command = sup::gui::GetGlobalCommandManager().RegisterCommand(command_id, command_id);

  // add underlying proxy action to the menu
  if (!sup::gui::AppAddActionToMenuBar(menu_name, command->GetProxyAction()))
  {
    throw RuntimeException("Global menu [" + menu_name.toStdString() + "] doesn't exist");
  }
  return command;
}

bool AppAddActionToCommand(QAction *action, const QString &command_id, const AppContext &context)
{
  // It will register new, or get access to already registered command, and add given action to its
  // list of real action.
  auto app_command =
      sup::gui::GetGlobalCommandManager().RegisterAction(action, command_id, context);
  return app_command != nullptr;
}

QAction *FindProxyAction(const QString &command_id)
{
  if (auto command = sup::gui::GetGlobalCommandManager().GetCommand(command_id); command)
  {
    return command->GetProxyAction();
  }

  return nullptr;
}

}  // namespace sup::gui
