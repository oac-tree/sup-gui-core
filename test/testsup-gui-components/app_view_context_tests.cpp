/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "sup/gui/app/app_view_context.h"

#include <sup/gui/app/null_command_service.h>
#include <sup/gui/settings/settings_manager.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for AppViewContext struct.
 */
class AppViewContextTest : public ::testing::Test
{
};

TEST_F(AppViewContextTest, InitialState)
{
  NullCommandService command_service;
  SettingsManager settings_manager;

  const AppViewContext context{command_service, settings_manager};

  EXPECT_EQ(&context.command_service, &command_service);
  EXPECT_EQ(&context.settings_manager, &settings_manager);
}

}  // namespace sup::gui::test
