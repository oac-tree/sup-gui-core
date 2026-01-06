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

#include "null_command_service.h"

#include <sup/gui/app/app_command_context.h>

namespace sup::gui
{

void NullCommandService::AppRegisterMenuBar(QMenuBar* menubar, const QStringList& top_menu_names)
{
  (void)menubar;
  (void)top_menu_names;
}

AppCommandContext NullCommandService::RegisterWidgetUniqueId(const QWidget* widget)
{
  (void)widget;
  return AppCommandContext({}, {});
}

void NullCommandService::UnregisterWidgetUniqueId(const QWidget* widget)
{
  (void)(widget);
}

QMenu* NullCommandService::AppGetMenu(const QString& menu_name)
{
  (void)menu_name;
  return nullptr;
}

bool NullCommandService::AddActionToMenuBar(const QString& menu_name, QAction* action)
{
  (void)menu_name;
  (void)action;
  return false;
}

AppCommand* NullCommandService::AddCommandToMenu(const QString& menu_name,
                                                 const QString& command_id)
{
  (void)menu_name;
  (void)command_id;
  return nullptr;
}

bool NullCommandService::AddActionToCommand(QAction* action, const QString& command_id,
                                            const AppCommandContext& context)
{
  (void)action;
  (void)command_id;
  (void)context;
  return false;
}

AppCommand* NullCommandService::GetCommand(const QString& command_id)
{
  (void)command_id;
  return nullptr;
}

void NullCommandService::OnFocusWidgetUpdate(QWidget* old, QWidget* now)
{
  (void)old;
  (void)now;
}

}  // namespace sup::gui
