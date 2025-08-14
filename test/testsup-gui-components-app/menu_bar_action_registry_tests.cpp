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

#include "sup/gui/app/menu_bar_action_registry.h"

#include <sup/gui/app/i_action_container.h>
#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <gtest/gtest.h>

#include <QAction>
#include <QMenu>
#include <QMenuBar>

namespace sup::gui::test
{

/**
 * @brief Tests for The MenuBarActionRegistry class.
 */
class MenuBarActionRegistryTest : public ::testing::Test
{
};

TEST_F(MenuBarActionRegistryTest, ActionManagerInitialState)
{
  MenuBarActionRegistry action_registry;
  EXPECT_EQ(action_registry.GetContainerCount(), 0);
  EXPECT_EQ(action_registry.GetContainer("abc"), nullptr);
  EXPECT_EQ(action_registry.GetMenuBar(), nullptr);
}

TEST_F(MenuBarActionRegistryTest, ActionManagerSetMenuBar)
{
  MenuBarActionRegistry registry;
  QMenuBar menubar;

  registry.SetMenuBar(&menubar);
  EXPECT_EQ(registry.GetMenuBar(), &menubar);
}

TEST_F(MenuBarActionRegistryTest, ActionManagerAddMenu)
{
  MenuBarActionRegistry registry;
  QMenuBar menubar;

  EXPECT_EQ(menubar.actions().size(), 0);

  registry.SetMenuBar(&menubar);

  // creating container with the menu
  auto container = registry.AddMenu("File");
  EXPECT_EQ(registry.GetContainerCount(), 1);
  EXPECT_EQ(registry.GetContainer("File"), container);

  // there is no harm to try to add the menu second time
  auto container2 = registry.AddMenu("File");
  EXPECT_EQ(container, container2);
  EXPECT_EQ(registry.GetContainerCount(), 1);

  // There is no way to check if menubar got a menu. We can check only an action associated with
  // the menu.
  EXPECT_EQ(menubar.actions().size(), 1);

  // We can get access to the menu via container. Menu was created, it has no actions yet.
  EXPECT_TRUE(container->GetMenu());
  EXPECT_EQ(container->GetActionCount(), 0);
}

TEST_F(MenuBarActionRegistryTest, Reset)
{
  MenuBarActionRegistry registry;
  QMenuBar menubar;

  EXPECT_EQ(menubar.actions().size(), 0);

  registry.SetMenuBar(&menubar);

  // creating container with the menu
  auto container = registry.AddMenu("File");
  EXPECT_EQ(registry.GetContainerCount(), 1);
  EXPECT_EQ(registry.GetMenuBar(), &menubar);

  registry.Reset();

  EXPECT_EQ(registry.GetMenuBar(), nullptr);
  EXPECT_EQ(registry.GetContainerCount(), 0);
}

TEST_F(MenuBarActionRegistryTest, AddMenuBarSecondTime)
{
  MenuBarActionRegistry registry;
  QMenuBar menubar;

  EXPECT_EQ(menubar.actions().size(), 0);

  registry.SetMenuBar(&menubar);

  // creating container with the menu
  auto container = registry.AddMenu("File");
  EXPECT_EQ(registry.GetContainerCount(), 1);
  EXPECT_EQ(registry.GetContainer("File"), container);

  EXPECT_NO_THROW(registry.SetMenuBar(&menubar));
  EXPECT_EQ(registry.GetContainerCount(), 1);
}

TEST_F(MenuBarActionRegistryTest, ActionManagerRegisterAction)
{
  MenuBarActionRegistry registry;
  QMenuBar menubar;

  EXPECT_EQ(menubar.actions().size(), 0);

  registry.SetMenuBar(&menubar);
  auto container = registry.AddMenu("File");

  // Registering user action.
  QAction action;
  EXPECT_TRUE(registry.RegisterAction("File", &action));

  // for the moment it is possible to add same action twice
  EXPECT_TRUE(registry.RegisterAction("File", &action));

  // it is not possible to add action to not-registered menu
  EXPECT_FALSE(registry.RegisterAction("Edit", &action));
}

}  // namespace sup::gui::test
