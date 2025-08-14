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

#include "sup/gui/app/menu_action_container.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <gtest/gtest.h>

#include <QAction>
#include <QMenu>

namespace sup::gui::test
{

class MenuActionContainerTest : public ::testing::Test
{
};

TEST_F(MenuActionContainerTest, MenuActionContainer)
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

}  // namespace sup::gui::test
