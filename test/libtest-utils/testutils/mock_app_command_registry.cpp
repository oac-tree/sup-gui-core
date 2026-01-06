/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#include "mock_app_command_registry.h"

namespace sup::gui::test
{

AppCommandRegistryDecorator::AppCommandRegistryDecorator(IAppCommandRegistry *registry)
    : m_registry(registry)
{
}

AppCommand *AppCommandRegistryDecorator::RegisterCommand(const QString &command_id,
                                                         const QString &command_text)
{
  return m_registry->RegisterCommand(command_id, command_text);
}

AppCommand *AppCommandRegistryDecorator::RegisterAction(QAction *action, const QString &command_id,
                                                        const AppCommandContext &context)
{
  return m_registry->RegisterAction(action, command_id, context);
}

AppCommand *AppCommandRegistryDecorator::GetCommand(const QString &command_id)
{
  return m_registry->GetCommand(command_id);
}

std::size_t AppCommandRegistryDecorator::GetCommandCount() const
{
  return m_registry->GetCommandCount();
}

void AppCommandRegistryDecorator::SetCurrentContext(const AppCommandContext &context)
{
  m_registry->SetCurrentContext(context);
}

void AppCommandRegistryDecorator::SetContextStack(const std::vector<AppCommandContext> &contexts)
{
  m_registry->SetContextStack(contexts);
}

}  // namespace sup::gui::test
