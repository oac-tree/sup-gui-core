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

#include "sup/gui/app/app_command_manager.h"

#include <sup/gui/app/app_command.h>
#include <sup/gui/widgets/proxy_action.h>

#include <gtest/gtest.h>

#include <QAction>
#include <QMenu>
#include <QMenuBar>

using namespace sup::gui;

class AppCommandManagerTest : public ::testing::Test
{
};

TEST_F(AppCommandManagerTest, InitialState)
{
  AppCommandManager manager(nullptr);

  EXPECT_EQ(manager.GetCommandCount(), 0);
  EXPECT_EQ(manager.GetCommand("Copy"), nullptr);
}

TEST_F(AppCommandManagerTest, RegisterCommand)
{
  AppCommandManager manager(nullptr);

  const QString context_name = "Editor.Copy";
  const QString command_text = "Copy";
  auto copy_command = manager.RegisterCommand(context_name, command_text);
  ASSERT_NE(copy_command, nullptr);

  EXPECT_EQ(copy_command->GetProxyAction()->text(), command_text);
  EXPECT_EQ(manager.GetCommandCount(), 1);
  EXPECT_EQ(manager.GetCommand(context_name), copy_command);

  // registering second time should return same command
  EXPECT_EQ(manager.RegisterCommand(context_name, command_text), copy_command);

  // Paste command wasn't yet registered
  EXPECT_EQ(manager.GetCommand("Paste"), nullptr);
}

TEST_F(AppCommandManagerTest, RegisterAction)
{
  const QKeySequence key("Ctrl+V");
  const QString command_text("Paste");
  const QString context_name("Editor.Paste");

  QAction paste_action1(command_text);
  QWidget widget1;
  QAction paste_action2(command_text);
  QWidget widget2;

  AppCommandManager manager(nullptr);

  auto command1 = manager.RegisterAction(context_name, &widget1, &paste_action1);
  command1->SetKeySequence(key);
  auto command2 = manager.RegisterAction(context_name, &widget2, &paste_action2);
  EXPECT_EQ(command1, command2);
}

TEST_F(AppCommandManagerTest, SetCurrentContextWidget)
{
  const QString command_text("Paste");
  const QString context_name("Editor.Paste");

  QAction paste_action1(command_text);
  QWidget widget1;
  QAction paste_action2(command_text);
  QWidget widget2;

  AppCommandManager manager(nullptr);

  auto command1 = manager.RegisterAction(context_name, &widget1, &paste_action1);
  auto command2 = manager.RegisterAction(context_name, &widget2, &paste_action2);
  EXPECT_EQ(command1, command2);

  EXPECT_EQ(command1->GetProxyAction()->GetAction(), nullptr);

  manager.SetCurrentContextWidget(&widget1);
  EXPECT_EQ(command1->GetProxyAction()->GetAction(), &paste_action1);
}
