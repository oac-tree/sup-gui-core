/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sup/gui/app/app_action_helper.h"

#include <sup/gui/app/app_action_manager.h>

#include <gtest/gtest.h>

#include <QAction>
#include <QMenu>
#include <QMenuBar>

using namespace sup::gui;

class AppActionHelperTest : public ::testing::Test
{
};

TEST_F(AppActionHelperTest, ActionManagerAddMenu)
{
  QMenuBar menubar;

  AppRegisterMenuBar(&menubar);

  auto container = AppAddMenu("File");

  // There is no way to check if menubar got a menu. We can check only an action associated with
  // the menu.
  EXPECT_EQ(menubar.actions().size(), 1);

  // We can get access to the menu via container. Menu was created, it has no actions yet.
  EXPECT_TRUE(container->GetMenu());
  EXPECT_EQ(container->GetMenu(), AppGetMenu("File"));
  EXPECT_EQ(container->GetActionCount(), 0);

  QAction action;
  EXPECT_TRUE(AppRegisterAction("File", &action));
}
