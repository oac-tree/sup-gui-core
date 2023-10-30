/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "app_action_helper.h"

#include "app_action_manager.h"

namespace sup::gui
{

ActionManager &GetGlobalActionManager()
{
  static ActionManager global_action_manager;
  return global_action_manager;
}

void AppRegisterMenuBar(QMenuBar *menubar)
{
  GetGlobalActionManager().SetMenuBar(menubar);
}

IActionContainer *AppAddMenu(const std::string &menu_name)
{
  return GetGlobalActionManager().AddMenu(menu_name);
}

bool AppRegisterAction(const std::string &menu_name, QAction *action)
{
  return GetGlobalActionManager().RegisterAction(menu_name, action);
}

bool AppRegisterActions(const std::string &menu_name, const QList<QAction *> &actions)
{
  bool result = true;
  for (auto action : actions)
  {
    result &= AppRegisterAction(menu_name, action);
  }
  return result;
}

QMenu *AppGetMenu(const std::string &menu_name)
{
  auto container = GetGlobalActionManager().GetContainer(menu_name);
  return container ? container->GetMenu() : nullptr;
}

}  // namespace sup::gui
