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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_APP_APP_ACTION_HELPER_H_
#define SUP_GUI_APP_APP_ACTION_HELPER_H_

//! @file
//! Collection of helper method to register global actions.

#include <sup/gui/app/app_context.h>

#include <QList>
#include <memory>

class QMenuBar;
class QAction;
class QMenu;

namespace sup::gui
{

class ActionManager;
class AppContextManager;
class AppCommandManager;
class IActionContainer;
class AppCommand;
class AppContextFocusController;

/**
 * @brief Returns global action manager.
 */
ActionManager& GetGlobalActionManager();

/**
 * @brief Returns global context manager.
 */
AppContextManager& GetGlobalContextManager();

/**
 * @brief Returns global command manager.
 */
AppCommandManager& GetGlobalCommandManager();

/**
 * @brief Creates focus controller to handle global proxy actions depending on current focus widget.
 */
std::unique_ptr<sup::gui::AppContextFocusController> CreateGlobalFocusController();

/**
 * @brief Registers the main menubar of QMainWindow to use in action manager.
 */
void AppRegisterMenuBar(QMenuBar* menubar);

/**
 * @brief Registers main menubar and create top level menus.
 *
 * @param menubar Main main window menubar.
 * @param names Vector of top level menus.
 */
void AppRegisterMenuBar(QMenuBar* menubar, const QStringList& names);

/**
 * @brief Adds menu to the registered menubar.
 */
IActionContainer* AppAddMenu(const QString& menu_name);

/**
 * @brief Returns top level menu registered in a menubar under this name.
 */
QMenu* AppGetMenu(const QString& menu_name);

/**
 * @brief Register and add action to the menu.
 */
bool AppRegisterAction(const QString& menu_name, QAction* action);

/**
 * @brief Register and add action to the menu.
 */
bool AppRegisterActions(const QString& menu_name, const QList<QAction*>& actions);

/**
 * @brief Registers given widget using unique identifier.
 *
 * This call generates a unique identifier (represented by AppContext class) and registers the
 * widget in the global database.
 *
 * @return The context representing used unique identifier.
 */
AppContext AppRegisterWidgetUniqueId(const QWidget* widget);

/**
 * @brief Unregisters given widget.
 *
 * Associated widget's unique context will be removed from the global database. It is assumed,
 * that the widget was registered earlier with AppRegisterWidgetUniqueId call.
 *
 * This method normally should be called in the destructor of the widget.
 */
void AppUnregisterWidgetUniqueId(const QWidget* widget);

/**
 * @brief Adds command to the menu.
 *
 * It will create a command, if necessary, and add its underlying proxy action to the menu.
 *
 * @param menu_name The name of the menu registered in AppActionManager.
 * @param command_id The id of the command which has proxy action.
 * @return Pointer to the command.
 */
AppCommand* AppAddCommandToMenu(const QString& menu_name, const QString& command_id);

/**
 * @brief Adds given action to command.
 *
 * @param action User action to add to the command.
 * @param command_id An identifier under which command is registered in AppCommandManager.
 * @param context The context of this command.
 */
bool AppAddActionToCommand(QAction* action, const QString& command_id, const AppContext& context);

/**
 * @brief Find proxy action corresponding to the given comman identifier.
 *
 * This action has been added to the main application menu using AppAddCommandToMenu.
 *
 * @param command_id An identifier under which command is registered in AppCommandManager.
 * @return Proxy action in the toolbar.
 */
QAction* FindProxyAction(const QString& command_id);

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_ACTION_HELPER_H_
