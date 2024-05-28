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

#ifndef SUP_GUI_APP_APP_CONTEXT_MANAGER_H_
#define SUP_GUI_APP_APP_CONTEXT_MANAGER_H_

#include <QObject>
#include <map>

namespace sup::gui
{

class AppCommand;

/**
 * @brief The AppCommandManager class holds a collection of commands that can be triggered either
 * via shortcuts or menus in the global taskbar.
 *
 * A command represents a proxy action that can be connected with other actions depending on the
 * current context.
 */
class AppCommandManager : public QObject
{
  Q_OBJECT

public:
  explicit AppCommandManager(QObject* parent);
  ~AppCommandManager() override;

  /**
   * @brief Registers a new command for a given context.
   *
   * Context is a name of a certain activity, e.g. "Copy", or "Undo". If the context doesn't exist,
   * the command will be created and stored in the manager. If context_name was already used, return
   * registered command to the user.
   *
   * @param context_name The name of the context.
   * @param command_text The text which will appear in the menu.
   *
   * @return Registered command.
   */
  AppCommand* RegisterCommand(const QString& context_name, const QString& command_text);

  /**
   * @brief Returns a command registered for a given context name.
   */
  AppCommand* GetCommand(const QString& context_name);

  /**
   * @brief Returns number of registered commands.
   */
  int GetCommandCount() const;

private:
  std::map<QString, AppCommand*> m_commands;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_CONTEXT_MANAGER_H_
