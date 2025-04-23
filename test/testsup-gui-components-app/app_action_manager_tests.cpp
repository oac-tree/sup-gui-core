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

#include "sup/gui/app/app_action_manager.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <gtest/gtest.h>

#include <QAction>
#include <QMenu>
#include <QMenuBar>

using namespace sup::gui;

class AppActionManagerTest : public ::testing::Test
{
};

TEST_F(AppActionManagerTest, MenuActionContainer)
{
  QMenu menu;

  MenuActionContainer container("abc", &menu);
  EXPECT_EQ(container.GetActionCount(), 0);
  EXPECT_EQ(container.GetMenu(), &menu);
  EXPECT_EQ(container.GetName(), QString("abc"));

  QAction action;
  EXPECT_TRUE(container.AddAction(&action));
  EXPECT_EQ(container.GetActionCount(), 1);
  ASSERT_EQ(menu.actions().size(), 1);
  ASSERT_EQ(menu.actions().at(0), &action);
}

TEST_F(AppActionManagerTest, ActionManagerInitialState)
{
  ActionManager manager;
  EXPECT_EQ(manager.GetContainerCount(), 0);
  EXPECT_EQ(manager.GetContainer("abc"), nullptr);
  EXPECT_EQ(manager.GetMenuBar(), nullptr);
}

TEST_F(AppActionManagerTest, ActionManagerSetMenuBar)
{
  ActionManager manager;
  QMenuBar menubar;

  manager.SetMenuBar(&menubar);
  EXPECT_EQ(manager.GetMenuBar(), &menubar);
}

TEST_F(AppActionManagerTest, ActionManagerAddMenu)
{
  ActionManager manager;
  QMenuBar menubar;

  EXPECT_EQ(menubar.actions().size(), 0);

  manager.SetMenuBar(&menubar);

  // creating container with the menu
  auto container = manager.AddMenu("File");
  EXPECT_EQ(manager.GetContainerCount(), 1);
  EXPECT_EQ(manager.GetContainer("File"), container);

  // there is no harm to try to add the menu second time
  auto container2 = manager.AddMenu("File");
  EXPECT_EQ(container, container2);
  EXPECT_EQ(manager.GetContainerCount(), 1);

  // There is no way to check if menubar got a menu. We can check only an action associated with
  // the menu.
  EXPECT_EQ(menubar.actions().size(), 1);

  // We can get access to the menu via container. Menu was created, it has no actions yet.
  EXPECT_TRUE(container->GetMenu());
  EXPECT_EQ(container->GetActionCount(), 0);
}

TEST_F(AppActionManagerTest, Reset)
{
  ActionManager manager;
  QMenuBar menubar;

  EXPECT_EQ(menubar.actions().size(), 0);

  manager.SetMenuBar(&menubar);

  // creating container with the menu
  auto container = manager.AddMenu("File");
  EXPECT_EQ(manager.GetContainerCount(), 1);
  EXPECT_EQ(manager.GetMenuBar(), &menubar);

  manager.Reset();

  EXPECT_EQ(manager.GetMenuBar(), nullptr);
  EXPECT_EQ(manager.GetContainerCount(), 0);
}

TEST_F(AppActionManagerTest, AddMenuBarSecondTime)
{
  ActionManager manager;
  QMenuBar menubar;

  EXPECT_EQ(menubar.actions().size(), 0);

  manager.SetMenuBar(&menubar);

  // creating container with the menu
  auto container = manager.AddMenu("File");
  EXPECT_EQ(manager.GetContainerCount(), 1);
  EXPECT_EQ(manager.GetContainer("File"), container);

  EXPECT_NO_THROW(manager.SetMenuBar(&menubar));
  EXPECT_EQ(manager.GetContainerCount(), 1);
}

TEST_F(AppActionManagerTest, ActionManagerRegisterAction)
{
  ActionManager manager;
  QMenuBar menubar;

  EXPECT_EQ(menubar.actions().size(), 0);

  manager.SetMenuBar(&menubar);
  auto container = manager.AddMenu("File");

  // Registering user action.
  QAction action;
  EXPECT_TRUE(manager.RegisterAction("File", &action));

  // for the moment it is possible to add same action twice
  EXPECT_TRUE(manager.RegisterAction("File", &action));

  // it is not possible to add action to not-registered menu
  EXPECT_FALSE(manager.RegisterAction("Edit", &action));
}
