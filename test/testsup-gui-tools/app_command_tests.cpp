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

#include <sup/gui/widgets/proxy_action.h>

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
  QWidget widget1;
  QWidget widget2;

  const AppContext context1("Paste", &widget1);
  const AppContext context2("Paste", &widget2);

  command.AddOverrideAction(context1, &real_action1);
  command.AddOverrideAction(context2, &real_action2);

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
}

//! Same as before, context is selected via focus widget.
TEST_F(AppCommandTest, SetCurrentContextWidget)
{
  const QKeySequence key("Ctrl+V");
  const QString expected_text("Paste");
  AppCommand command(expected_text, key);

  const QString expected_name1("paste-from-widget1");
  const QString expected_name2("paste-from-widget2");
  QAction real_action1(expected_name1);
  QAction real_action2(expected_name2);
  QWidget widget1;
  QWidget widget2;

  const AppContext context1("Paste", &widget1);
  const AppContext context2("Paste", &widget2);

  command.AddOverrideAction(context1, &real_action1);
  command.AddOverrideAction(context2, &real_action2);

  // there is underlying proxy action, but it is unconnected
  ASSERT_NE(command.GetProxyAction(), nullptr);
  EXPECT_FALSE(command.GetProxyAction()->isEnabled());
  EXPECT_EQ(command.GetProxyAction()->text(), expected_text);
  EXPECT_EQ(command.GetProxyAction()->GetAction(), nullptr);

  // setting context, proxy now is looking to second action
  command.SetCurrentContextWidget(&widget2);
  EXPECT_TRUE(command.GetProxyAction()->isEnabled());
  EXPECT_EQ(command.GetProxyAction()->text(), expected_name2);
  EXPECT_EQ(command.GetProxyAction()->GetAction(), &real_action2);
}
