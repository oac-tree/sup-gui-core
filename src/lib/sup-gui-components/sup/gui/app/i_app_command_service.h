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

#ifndef SUP_GUI_APP_I_APP_COMMAND_SERVICE_H_
#define SUP_GUI_APP_I_APP_COMMAND_SERVICE_H_

#include <QVector>

class QWidget;
class QString;
class QMenuBar;
class QAction;
class QMenu;

namespace sup::gui
{

class AppCommand;
class AppCommandContext;

/**
 * @brief The IAppCommandService interface allows to register commands, subscribe multiple actions
 * to it and execute some of them depending on the current context.
 *
 * A command (AppCommand class) is a proxy action that can be forwarded to other actions depending
 * on the current context.
 *
 * A context (AppCommandContext) is an unique itendifier for widget. When widget is in focus,
 */
class IAppCommandService
{
public:
  IAppCommandService() = default;
  virtual ~IAppCommandService() = default;

  IAppCommandService(const IAppCommandService&) = delete;
  IAppCommandService& operator=(const IAppCommandService&) = delete;
  IAppCommandService(IAppCommandService&&) = delete;
  IAppCommandService& operator=(IAppCommandService&&) = delete;

  /**
   * @brief Registers given widget using unique identifier.
   *
   * This call generates a unique identifier (represented by AppContext class) and registers the
   * widget in the global database.
   *
   * @return The context representing used unique identifier.
   */
  virtual AppCommandContext RegisterWidgetUniqueId(const QWidget* widget) = 0;

  /**
   * @brief Unregisters given widget.
   *
   * Associated widget's unique context will be removed from the global database. It is assumed,
   * that the widget was registered earlier with AppRegisterWidgetUniqueId call. This method
   * normally should be called in the destructor of the widget.
   */
  virtual void UnregisterWidgetUniqueId(const QWidget* widget) = 0;

  /**
   * @brief Returns top level menu registered in a menubar under this name.
   */
  virtual QMenu* AppGetMenu(const QString& menu_name) = 0;

  /**
   * @brief Add action to the global menu bar.
   *
   * The action will be appended to menu with the given name. The ownership of the action
   * is not taken.
   *
   * @param menu_name The name of already existing menu added via AddMenu call.
   * @param action User action to add.
   *
   * @return True if action was added, or false if no such menu was registered.
   */
  virtual bool AddActionToMenuBar(const QString& menu_name, QAction* action) = 0;

  /**
   * @brief Adds command to the menu.
   *
   * It will create a command, if necessary, and add its underlying proxy action to the menu.
   *
   * @param menu_name The name of the menu registered in application-wide menubar.
   * @param command_id The id of the command which has proxy action.
   * @return Pointer to the command.
   */
  virtual AppCommand* AddCommandToMenu(const QString& menu_name, const QString& command_id) = 0;

  /**
   * @brief Adds given action to command.
   *
   * It will register new, or get access to already registered command, and add given action to its
   * list of real action.
   *
   * @param action User action to add to the command.
   * @param command_id An identifier under which command is registered in IAppCommandRegistry.
   * @param context The context of this command.
   */
  virtual bool AddActionToCommand(QAction* action, const QString& command_id,
                                  const AppCommandContext& context) = 0;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_I_APP_COMMAND_SERVICE_H_
