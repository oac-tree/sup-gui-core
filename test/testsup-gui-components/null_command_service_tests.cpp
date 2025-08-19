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

#include "sup/gui/app/null_command_service.h"

#include <sup/gui/app/app_command.h>
#include <sup/gui/app/menu_action_container.h>
#include <sup/gui/components/proxy_action.h>

#include <gtest/gtest.h>

#include <QMenu>
#include <QWidget>

namespace sup::gui::test
{

/**
 * @brief Tests of NullCommandService class.
 */
class NullCommandServiceTest : public ::testing::Test
{
};

TEST_F(NullCommandServiceTest, InitialState)
{
  NullCommandService null_service;
  EXPECT_NO_FATAL_FAILURE(null_service.AppRegisterMenuBar(nullptr, {}));
  EXPECT_EQ(null_service.RegisterWidgetUniqueId(nullptr), AppCommandContext({}, {}));
  EXPECT_NO_FATAL_FAILURE(null_service.UnregisterWidgetUniqueId(nullptr));
  EXPECT_EQ(null_service.AppGetMenu("TestMenu"), nullptr);
  EXPECT_FALSE(null_service.AddActionToMenuBar("TestMenu", nullptr));
  EXPECT_EQ(null_service.AddCommandToMenu("TestMenu", "TestCommand"), nullptr);
  EXPECT_FALSE(null_service.AddActionToCommand(nullptr, "TestCommand", AppCommandContext({}, {})));
  EXPECT_EQ(null_service.GetCommand("TestCommand"), nullptr);
}

}  // namespace sup::gui::test
