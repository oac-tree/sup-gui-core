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

#ifndef SUP_GUI_APP_APP_ACTION_MANAGER_H_
#define SUP_GUI_APP_APP_ACTION_MANAGER_H_

#include <QString>
#include <QVector>
#include <map>
#include <memory>
#include <string>
#include <vector>

class QAction;
class QMenuBar;
class QMenu;

namespace sup::gui
{

class IActionContainer
{
public:
  virtual ~IActionContainer() = default;

  /**
   * @brief Returns number of actions registered in this container.
   */
  virtual size_t GetActionCount() = 0;

  /**
   * @brief The menu associated with this container, where actionms are added.
   */
  virtual QMenu* GetMenu() = 0;

  /**
   * @brief Adds action to the menu associated with this container.
   *
   * @param The action to add (ownership belongs to the user).
   * @return True if action has been successfully added.
   */
  virtual bool AddAction(QAction* action) = 0;
};

/**
 * @brief The MenuActionContainer class holds information about a menu and actions added to it.
 */
class MenuActionContainer : public IActionContainer
{
public:
  /**
   * @brief Main constructor.
   *
   * @param name The name of the container.
   * @param menu The menu to serve.
   */
  MenuActionContainer(const QString& name, QMenu* menu);

  QString GetName() { return m_name; }

  QMenu* GetMenu() override { return m_menu; }

  bool AddAction(QAction* action) override;

  size_t GetActionCount() override;

private:
  QString m_name;
  QMenu* m_menu{nullptr};
  std::vector<QAction*> m_actions;
};

class IActionManager
{
public:
  virtual ~IActionManager() = default;
  virtual bool RegisterAction(const QString& group_name, QAction* action) = 0;
};

/**
 * @brief The ActionManager class provides subscriptions of user actions to the menubar of the main
 * window.
 *
 * @details It doesn't own user actions. Intended to be used as a global variable.
 */

class ActionManager : public IActionManager
{
public:
  /**
   * @brief Returns menubar served by this manager.
   */
  QMenuBar* GetMenuBar();

  /**
   * @brief Sets menubar to serve.
   *
   * @details There can be only one menubar to serve.
   */
  void SetMenuBar(QMenuBar* menubar);

  /**
   * @brief Adds new menu to the menu bar.
   *
   * @param menu_name The title of the menu.
   * @param menubar The menubar where the menu will be added.
   *
   * @return Action container holding just created menu and other information.
   *
   * @details The menubar will take an ownership. If menu was already created, just returns action
   * container.
   */
  IActionContainer* AddMenu(const QString& menu_name);

  /**
   * @brief Registers action and add it to the menu.
   *
   * @param menu_name The name of already existing menu added via AddMenu call.
   * @param action User action to add.
   * @return True if action was added, or false if no such menu was registered.
   *
   * @details The ownership of the action is not taken.
   */
  bool RegisterAction(const QString& menu_name, QAction* action) override;

  IActionContainer* GetContainer(const QString& menu_name);

  /**
   * @brief Returns the number of registered main containers.
   *
   *
   */
  size_t GetContainerCount() const;

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
