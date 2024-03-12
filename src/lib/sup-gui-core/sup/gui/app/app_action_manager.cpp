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

#include "app_action_manager.h"

#include <sup/gui/core/exceptions.h>

#include <QAction>
#include <QMenu>
#include <QMenuBar>

namespace sup::gui
{

MenuActionContainer::MenuActionContainer(const std::string &name, QMenu *menu)
    : m_menu(menu), m_name(name)
{
  if (!m_menu)
  {
    throw LogicErrorException("Menu is not initialised");
  }
}

bool MenuActionContainer::AddAction(QAction *action)
{
  m_actions.push_back(action);
  m_menu->addAction(action);
  return true;
}

size_t MenuActionContainer::GetActionCount()
{
  return m_actions.size();
}

// ----------------------------------------------------------------------------
// ActionManager
// ----------------------------------------------------------------------------

QMenuBar *ActionManager::GetMenuBar()
{
  return m_menubar;
}

void ActionManager::SetMenuBar(QMenuBar *menubar)
{
  if (!menubar)
  {
    throw RuntimeException("Attempt to set uninitialized menubar");
  }

  if (!m_menubar)
  {
    // registering menubar
    m_menubar = menubar;
    return;
  }

  if (m_menubar == menubar)
  {
    // nothing to do, same menubar
    return;
  }

  throw RuntimeException("There can be only one application minibar");
}

IActionContainer *ActionManager::AddMenu(const std::string &menu_name)
{
  ValidateMenuBar();

  if (auto container = GetContainer(menu_name); container)
  {
    return container;
  }

  auto menu = m_menubar->addMenu(QString::fromStdString(menu_name));
  menu->setToolTipsVisible(true);
  auto container = std::make_unique<MenuActionContainer>(menu_name, menu);
  auto container_ptr = container.get();
  m_action_storage.insert({menu_name, std::move(container)});
  return container_ptr;
}

bool ActionManager::RegisterAction(const std::string &menu_name, QAction *action)
{
  if (auto container = GetContainer(menu_name); container)
  {
    return container->AddAction(action);
  }

  return false;
}

IActionContainer *ActionManager::GetContainer(const std::string &menu_name)
{
  auto iter = m_action_storage.find(menu_name);
  return iter == m_action_storage.end() ? nullptr : iter->second.get();
}

size_t ActionManager::GetContainerCount() const
{
  return m_action_storage.size();
}

void ActionManager::ValidateMenuBar()
{
  if (!m_menubar)
  {
    throw LogicErrorException("No menubar was initialised");
  }
}

}  // namespace sup::gui
