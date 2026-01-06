/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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
 * @brief The MenuBarActionRegistry class provides subscriptions of user actions to the menubar of
 * the main window.
 *
 * It doesn't own user actions. Intended to be used as a global variable.
 */
class MenuBarActionRegistry : public IActionContainerRegistry
{
public:
  MenuBarActionRegistry() = default;
  ~MenuBarActionRegistry() override;

  MenuBarActionRegistry(const MenuBarActionRegistry&) = delete;
  MenuBarActionRegistry& operator=(const MenuBarActionRegistry&) = delete;
  MenuBarActionRegistry(MenuBarActionRegistry&&) = delete;
  MenuBarActionRegistry& operator=(MenuBarActionRegistry&&) = delete;

  /**
   * @brief Returns menubar served by this manager.
   */
  QMenuBar* GetMenuBar();

  void SetMenuBar(QMenuBar* menubar) override;

  IActionContainer* AddMenu(const QString& menu_name) override;

  bool RegisterAction(const QString& menu_name, QAction* action) override;

  IActionContainer* GetContainer(const QString& menu_name) override;

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

#endif  // SUP_GUI_APP_MENU_BAR_ACTION_REGISTRY_H_
