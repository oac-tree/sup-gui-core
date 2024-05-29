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

#ifndef SUP_GUI_APP_APP_ACTION_HELPER_H_
#define SUP_GUI_APP_APP_ACTION_HELPER_H_

//! @file
//! Collection of helper method to register global actions.

#include <sup/gui/app/app_context.h>

#include <QList>
#include <string>

class QMenuBar;
class QAction;
class QMenu;

namespace sup::gui
{

namespace constants
{
//!< the name of the groups corresponding to main menus of the menubar

static inline const std::string kFileMenu = "&File";
static inline const std::string kEditMenu = "&Edit";
static inline const std::string kToolsMenu = "&Tools";
static inline const std::string kViewMenu = "&View";
static inline const std::string kHelpMenu = "&Help";
}  // namespace constants

class ActionManager;
class AppContextManager;
class AppCommandManager;
class IActionContainer;
class AppCommand;

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
 * @brief Registers the main menubar of QMainWindow to use in action manager.
 */
void AppRegisterMenuBar(QMenuBar* menubar);

/**
 * @brief Adds menu to the registered menubar.
 */
IActionContainer* AppAddMenu(const std::string& menu_name);

/**
 * @brief Register and add action to the menu.
 */
bool AppRegisterAction(const std::string& menu_name, QAction* action);

/**
 * @brief Register and add action to the menu.
 */
bool AppRegisterActions(const std::string& menu_name, const QList<QAction*>& actions);

/**
 * @brief Returns top level menu registered in a menubar under this name.
 */
QMenu* AppGetMenu(const std::string& menu_name);

/**
 * @brief Registers main menubar and create top level menus.
 *
 * @param menubar Main main window menubar.
 * @param names Vector of top level menus.
 */
void AppRegisterMainMenuBar(QMenuBar* menubar, const std::vector<std::string>& names);

/**
 * @brief Registers context for widget.
 */
void AppRegisterContext(const QWidget* widget, const AppContext& context);

/**
 * @brief Adds proxy action to the menu.
 *
 * @param menu_name The name of the menu registered in AppActionManager.
 * @param command_id The id of the command which has proxy action.
 * @return Pointer to the command.
 */
AppCommand* AppAddProxyAction(const std::string& menu_name, const QString& command_id);

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_ACTION_HELPER_H_
