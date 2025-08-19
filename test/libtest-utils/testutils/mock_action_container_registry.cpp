/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#include "mock_action_container_registry.h"

namespace sup::gui::test
{
ActionContainerRegistryDecorator::ActionContainerRegistryDecorator(
    IActionContainerRegistry *registry)
    : m_registry(registry)
{
}

void ActionContainerRegistryDecorator::SetMenuBar(QMenuBar *menubar)
{
  m_registry->SetMenuBar(menubar);
}

IActionContainer *ActionContainerRegistryDecorator::AddMenu(const QString &menu_name)
{
  return m_registry->AddMenu(menu_name);
}

bool ActionContainerRegistryDecorator::RegisterAction(const QString &container_name,
                                                      QAction *action)
{
  return m_registry->RegisterAction(container_name, action);
}

}  // namespace sup::gui::test
