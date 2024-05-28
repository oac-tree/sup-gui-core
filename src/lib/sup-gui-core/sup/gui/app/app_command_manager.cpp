/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "app_command_manager.h"

#include "app_command.h"

#include <QAction>

namespace sup::gui
{

AppCommandManager::AppCommandManager(QObject *parent) : QObject(parent) {}

AppCommandManager::~AppCommandManager() = default;

AppCommand *AppCommandManager::RegisterCommand(const QString &context_name,
                                               const QString &command_text)
{
  if (auto command = GetCommand(context_name); command)
  {
    // existing command
    return command;
  }

  auto command = new AppCommand(command_text, this);  // ownership belongs to the manager
  m_commands.insert({context_name, command});
  return command;
}

AppCommand *AppCommandManager::RegisterAction(const QString &context_name, QWidget *widget,
                                              QAction *action)
{
  auto command = RegisterCommand(context_name, action->text());
  Q_ASSERT(command);

  command->AddOverrideAction(AppContext{context_name, widget}, action);
  return command;
}

AppCommand *AppCommandManager::GetCommand(const QString &context_name)
{
  auto iter = m_commands.find(context_name);
  return iter == m_commands.end() ? nullptr : iter->second;
}

int AppCommandManager::GetCommandCount() const
{
  return static_cast<int>(m_commands.size());
}

}  // namespace sup::gui
