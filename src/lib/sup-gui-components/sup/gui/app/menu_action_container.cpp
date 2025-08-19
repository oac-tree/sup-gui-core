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

#include "menu_action_container.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <QAction>
#include <QMenu>

namespace sup::gui
{

MenuActionContainer::MenuActionContainer(const QString &name, QMenu *menu)
    : m_name(name), m_menu(menu)
{
  if (!m_menu)
  {
    throw LogicErrorException("Menu is not initialised");
  }
}

QString MenuActionContainer::GetName() { return m_name; }

QMenu *MenuActionContainer::GetMenu() { return m_menu; }

bool MenuActionContainer::AddAction(QAction *action)
{
  m_actions.push_back(action);
  m_menu->addAction(action);
  return true;
}

std::size_t MenuActionContainer::GetActionCount()
{
  return m_actions.size();
}

}  // namespace sup::gui
