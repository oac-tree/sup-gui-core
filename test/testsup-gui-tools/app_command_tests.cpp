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
  QKeySequence key("Ctrl+V");

  const QString expected_text("Paste");
  AppCommand command(expected_text, key);

  // there is underlying proxy action, but it is unconnected
  ASSERT_NE(command.GetProxyAction(), nullptr);
  EXPECT_EQ(command.GetProxyAction()->text(), expected_text);
  EXPECT_EQ(command.GetProxyAction()->GetAction(), nullptr);
}
