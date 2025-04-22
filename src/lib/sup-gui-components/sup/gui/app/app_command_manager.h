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

#ifndef SUP_GUI_APP_APP_COMMAND_MANAGER_H_
#define SUP_GUI_APP_APP_COMMAND_MANAGER_H_

#include <QString>
#include <map>
#include <memory>

class QAction;

namespace sup::gui
{

class AppCommand;
class AppContext;

/**
 * @brief The AppCommandManager class holds a collection of commands that can be triggered either
 * via shortcuts or menus in the global taskbar.
 *
 * A command represents a proxy action that can be connected with other actions depending on the
 * current context.
 */
class AppCommandManager
{
public:
  /**
   * @brief Registers a new command for a given id.
   *
   * If the command_id is new to the manager, the command will be created and stored in the manager.
   * If command_id was already used, return registered command to the user.
   *
   * @param command_id The id of the command.
   * @param command_text The text which will appear in the menu.
   *
   * @return Registered command.
   */
  AppCommand* RegisterCommand(const QString& command_id, const QString& command_text = {});

  /**
   * @brief Registers an action for a given command ID using a context.
   *
   * Internally creates a command, if necessary, and adds an action to the list of command actions.
   * The action will be associated with the given context. Action ownership remains on
   * the user side.
   *
   * @param action An action to register.
   * @param command_id The ID of the command.
   * @param context Command context.
   * @return The command that will trigger the action.
   */
  AppCommand* RegisterAction(QAction* action, const QString& command_id, const AppContext& context);

  /**
   * @brief Returns a command registered for a given command_id.
   */
  AppCommand* GetCommand(const QString& command_id);

  /**
   * @brief Returns number of registered commands.
   */
  int GetCommandCount() const;

  /**
   * @brief Sets the given context for all registered commands.
   */
  void SetCurrentContext(const AppContext& context);

  /**
   * @brief Sets the given context stack for all registered commands.
   */
  void SetContextStack(const std::vector<AppContext>& context_stack);

private:
  //!< correspondence of the command_id to commands
  std::map<QString, std::unique_ptr<AppCommand>> m_commands;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_COMMAND_MANAGER_H_
