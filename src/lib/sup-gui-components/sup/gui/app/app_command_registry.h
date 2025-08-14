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

#ifndef SUP_GUI_APP_APP_COMMAND_REGISTRY_H_
#define SUP_GUI_APP_APP_COMMAND_REGISTRY_H_

#include <sup/gui/app/i_app_command_registry.h>

#include <map>
#include <memory>

namespace sup::gui
{

/**
 * @brief The AppCommandRegistry class holds a collection of commands that can be triggered either
 * via shortcuts or menus in the global taskbar.
 *
 * A command represents a proxy action that can be connected with other actions depending on the
 * current context.
 */
class AppCommandRegistry : public IAppCommandRegistry
{
public:
  AppCommandRegistry();
  ~AppCommandRegistry() override;

  AppCommandRegistry(const AppCommandRegistry&) = delete;
  AppCommandRegistry& operator=(const AppCommandRegistry&) = delete;
  AppCommandRegistry(AppCommandRegistry&&) = delete;
  AppCommandRegistry& operator=(AppCommandRegistry&&) = delete;

  AppCommand* RegisterCommand(const QString& command_id, const QString& command_text) override;

  AppCommand* RegisterAction(QAction* action, const QString& command_id,
                             const AppCommandContext& context) override;

  AppCommand* GetCommand(const QString& command_id) override;

  std::size_t GetCommandCount() const override;

  void SetCurrentContext(const AppCommandContext& context) override;

  void SetContextStack(const std::vector<AppCommandContext>& context_stack) override;

private:
  //!< correspondence of the command_id to commands
  std::map<QString, std::unique_ptr<AppCommand>> m_commands;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_COMMAND_REGISTRY_H_
