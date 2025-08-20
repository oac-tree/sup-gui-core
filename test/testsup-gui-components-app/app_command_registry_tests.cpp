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

#include "sup/gui/app/app_command_registry.h"

#include <sup/gui/app/app_command.h>
#include <sup/gui/component_objects/proxy_action.h>

#include <gtest/gtest.h>

#include <QAction>
#include <QMenu>
#include <QMenuBar>

namespace sup::gui::test
{

class AppCommandRegistryTest : public ::testing::Test
{
};

TEST_F(AppCommandRegistryTest, InitialState)
{
  AppCommandRegistry registry;

  EXPECT_EQ(registry.GetCommandCount(), 0);
  EXPECT_EQ(registry.GetCommand("Copy"), nullptr);
}

TEST_F(AppCommandRegistryTest, RegisterCommand)
{
  AppCommandRegistry registry;

  const QString context_name = "Editor.Copy";
  const QString command_text = "Copy";
  auto copy_command = registry.RegisterCommand(context_name, command_text);
  ASSERT_NE(copy_command, nullptr);

  EXPECT_EQ(copy_command->GetProxyAction()->text(), command_text);
  EXPECT_EQ(registry.GetCommandCount(), 1);
  EXPECT_EQ(registry.GetCommand(context_name), copy_command);

  // registering second time should return same command
  EXPECT_EQ(registry.RegisterCommand(context_name, command_text), copy_command);

  // Paste command wasn't yet registered
  EXPECT_EQ(registry.GetCommand("Paste"), nullptr);
}

TEST_F(AppCommandRegistryTest, RegisterAction)
{
  const QKeySequence key("Ctrl+V");
  const QString command_text("Paste");
  const QString command_id("Editor.Paste");

  QAction paste_action1(command_text);
  QAction paste_action2(command_text);

  const AppCommandContext context1("Editor1.Paste");
  const AppCommandContext context2("Editor2.Paste");

  AppCommandRegistry registry;

  auto command1 = registry.RegisterAction(&paste_action1, command_id, context1);
  command1->SetShortcut(key);
  auto command2 = registry.RegisterAction(&paste_action2, command_id, context2);
  EXPECT_EQ(command1, command2);

  EXPECT_EQ(command1->GetProxyAction()->GetAction(), nullptr);

  registry.SetCurrentContext(context1);
  EXPECT_EQ(command1->GetProxyAction()->GetAction(), &paste_action1);
}

TEST_F(AppCommandRegistryTest, SetContextStack)
{
  const QKeySequence key("Ctrl+V");
  const QString command_text("Paste");
  const QString command_id("Editor.Paste");

  QAction paste_action1(command_text);
  QAction paste_action2(command_text);

  const AppCommandContext context1("Editor1.Paste");
  const AppCommandContext context2("Editor2.Paste");

  AppCommandRegistry registry;

  auto command1 = registry.RegisterAction(&paste_action1, command_id, context1);
  command1->SetShortcut(key);
  auto command2 = registry.RegisterAction(&paste_action2, command_id, context2);
  EXPECT_EQ(command1, command2);

  EXPECT_EQ(command1->GetProxyAction()->GetAction(), nullptr);

  const AppCommandContext some_parent_context("parent-context");
  registry.SetContextStack({some_parent_context, context1});
  EXPECT_EQ(command1->GetProxyAction()->GetAction(), &paste_action1);
}

}  // namespace sup::gui::test
