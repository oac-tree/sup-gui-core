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

#include "sup/gui/app/app_command.h"

#include <sup/gui/core/exceptions.h>
#include <sup/gui/components/proxy_action.h>

#include <gtest/gtest.h>

#include <QWidget>

using namespace sup::gui;

class AppCommandTest : public ::testing::Test
{
};

TEST_F(AppCommandTest, InitialState)
{
  const QKeySequence key("Ctrl+V");

  const QString expected_text("Paste");
  AppCommand command(expected_text, key);

  // there is underlying proxy action, but it is unconnected
  ASSERT_NE(command.GetProxyAction(), nullptr);
  EXPECT_EQ(command.GetProxyAction()->text(), expected_text);
  EXPECT_EQ(command.GetProxyAction()->GetAction(), nullptr);
  EXPECT_EQ(command.GetActionForContext(AppContext{}), nullptr);
  EXPECT_FALSE(command.HasAction(AppContext{}));
}

//! Two actions related to two different widgets are added to the same command.
//! Checking that by setting the context, a proxy action is pointed to the correct real action.
TEST_F(AppCommandTest, AddOverrideAction)
{
  const QKeySequence key("Ctrl+V");
  const QString expected_text("Paste");
  AppCommand command(expected_text, key);

  const QString expected_name1("paste-from-widget1");
  const QString expected_name2("paste-from-widget2");
  QAction real_action1(expected_name1);
  QAction real_action2(expected_name2);

  const AppContext context1("Editor1.Paste");
  const AppContext context2("Editor2.Paste");

  command.AddOverrideAction(context1, &real_action1);
  command.AddOverrideAction(context2, &real_action2);

  EXPECT_EQ(command.GetActionForContext(context1), &real_action1);
  EXPECT_TRUE(command.HasAction(context1));
  EXPECT_EQ(command.GetActionForContext(context2), &real_action2);
  EXPECT_TRUE(command.HasAction(context2));

  // there is underlying proxy action, but it is unconnected
  ASSERT_NE(command.GetProxyAction(), nullptr);
  EXPECT_FALSE(command.GetProxyAction()->isEnabled());
  EXPECT_EQ(command.GetProxyAction()->text(), expected_text);
  EXPECT_EQ(command.GetProxyAction()->GetAction(), nullptr);

  // setting context, proxy now is looking to second action
  command.SetCurrentContext(context2);
  EXPECT_TRUE(command.GetProxyAction()->isEnabled());
  EXPECT_EQ(command.GetProxyAction()->text(), expected_name2);
  EXPECT_EQ(command.GetProxyAction()->GetAction(), &real_action2);

  // setting context that doesn't exist
  command.SetCurrentContext(AppContext());
  EXPECT_FALSE(command.GetProxyAction()->isEnabled());

  // attempt to register another action with same context
  QAction real_action3("another-action");
  EXPECT_THROW(command.AddOverrideAction(context2, &real_action3), RuntimeException);
}

//! Validating that proxy action gets its default text, after we set non-existing context.
TEST_F(AppCommandTest, SetCurrentContext)
{
  const QKeySequence key("Ctrl+V");
  const QString expected_text("Default Text");
  AppCommand command(expected_text, key);

  const QString real_action_text("paste-from-widget1");
  QAction real_action(real_action_text);
  const AppContext context("Editor.Paste");

  // default placeholder text
  EXPECT_EQ(command.GetProxyAction()->text(), expected_text);

  command.AddOverrideAction(context, &real_action);

  // still default placeholder text
  EXPECT_EQ(command.GetProxyAction()->text(), expected_text);

  command.SetCurrentContext(context);
  EXPECT_EQ(command.GetProxyAction()->text(), real_action_text);

  const AppContext non_existing_context("non_existing_context");
  command.SetCurrentContext(non_existing_context);

  // text returned back to the default placeholder text
  EXPECT_EQ(command.GetProxyAction()->text(), expected_text);
}

//! Setting context stack. Action should be enabled if registered context is in the stack.
TEST_F(AppCommandTest, SetContextStack)
{
  const QString expected_text("Default Text");
  AppCommand command(expected_text);

  const QString real_action_text("paste-from-widget");
  QAction real_action(real_action_text);
  const AppContext context("Editor.Paste");
  command.AddOverrideAction(context, &real_action);

  const AppContext parent_context("Parent");

  command.SetContextStack({parent_context, context});
  EXPECT_EQ(command.GetProxyAction()->GetAction(), &real_action);
}

//! Setting context stack. Action should be disabled.
TEST_F(AppCommandTest, SetEmptyContextStack)
{
  const QString expected_text("Default Text");
  AppCommand command(expected_text);

  const QString real_action_text("paste-from-widget");
  QAction real_action(real_action_text);
  const AppContext context("Editor.Paste");
  command.AddOverrideAction(context, &real_action);

  command.SetContextStack({});
  EXPECT_EQ(command.GetProxyAction()->GetAction(), nullptr);
}
