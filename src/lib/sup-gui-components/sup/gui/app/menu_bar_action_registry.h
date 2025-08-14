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

#ifndef SUP_GUI_APP_MENU_BAR_ACTION_REGISTRY_H_
#define SUP_GUI_APP_MENU_BAR_ACTION_REGISTRY_H_

#include <sup/gui/app/i_action_container_registry.h>

#include <QVector>
#include <map>
#include <memory>

class QAction;
class QMenuBar;
class QMenu;

namespace sup::gui
{

class IActionContainer;

/**
 * @brief The ActionManager class provides subscriptions of user actions to the menubar of the main
 * window.
 *
 * It doesn't own user actions. Intended to be used as a global variable.
 */
class MenuBarActionRegistry : public IActionContainerRegistry
{
public:
  /**
   * @brief Returns menubar served by this manager.
   */
  QMenuBar* GetMenuBar();

  /**
   * @brief Sets menubar to serve.
   *
   * There can be only one menubar to serve.
   */
  void SetMenuBar(QMenuBar* menubar);

  /**
   * @brief Adds new menu to the menu bar.
   *
   * The menubar will take an ownership. If menu was already created, just returns action container.
   *
   * @param menu_name The title of the menu.
   * @param menubar The menubar where the menu will be added.
   *
   * @return Action container holding just created menu and other information.
   */
  IActionContainer* AddMenu(const QString& menu_name);

  /**
   * @brief Registers action and add it to the menu.
   *
   * The ownership of the action is not taken.
   *
   * @param menu_name The name of already existing menu added via AddMenu call.
   * @param action User action to add.
   * @return True if action was added, or false if no such menu was registered.
   */
  bool RegisterAction(const QString& menu_name, QAction* action) override;

  IActionContainer* GetContainer(const QString& menu_name);

  /**
   * @brief Returns the number of registered main containers.
   */
  std::size_t GetContainerCount() const;

  /**
   * @brief Resets all information about registered menus and main menubar.
   *
   * This is method can be called when application is going to be restarted. Newly started
   * application will register it's own menubar.
   */
  void Reset();

private:
  void ValidateMenuBar();

  std::map<QString, std::unique_ptr<IActionContainer>> m_action_storage;
  QMenuBar* m_menubar{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_ACTION_MANAGER_H_
