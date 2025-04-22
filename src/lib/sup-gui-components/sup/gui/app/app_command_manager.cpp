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

#include "app_command_manager.h"

#include "app_command.h"

#include <QAction>

namespace sup::gui
{

AppCommand *AppCommandManager::RegisterCommand(const QString &command_id,
                                               const QString &command_text)
{
  if (auto command = GetCommand(command_id); command)
  {
    return command;  // existing command
  }

  auto command = std::make_unique<AppCommand>(command_text, nullptr);
  auto command_ptr = command.get();
  (void)m_commands.insert({command_id, std::move(command)});
  return command_ptr;
}

AppCommand *AppCommandManager::RegisterAction(QAction *action, const QString &command_id,
                                              const AppContext &context)
{
  auto command = RegisterCommand(command_id, action->text());
  Q_ASSERT(command);

  command->AddOverrideAction(context, action);
  return command;
}

AppCommand *AppCommandManager::GetCommand(const QString &command_id)
{
  auto iter = m_commands.find(command_id);
  return iter == m_commands.end() ? nullptr : iter->second.get();
}

int AppCommandManager::GetCommandCount() const
{
  return static_cast<int>(m_commands.size());
}

void AppCommandManager::SetCurrentContext(const AppContext &context)
{
  for (const auto &[command_id, command] : m_commands)
  {
    command->SetCurrentContext(context);
  }
}

void AppCommandManager::SetContextStack(const std::vector<AppContext> &context_stack)
{
  for (const auto &[command_id, command] : m_commands)
  {
    command->SetContextStack(context_stack);
  }
}

}  // namespace sup::gui
