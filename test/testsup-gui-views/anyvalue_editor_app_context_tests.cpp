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

#include "sup/gui/mainwindow/anyvalue_editor_app_context.h"

#include <sup/gui/app/app_command_service.h>
#include <sup/gui/app/menu_bar_action_registry.h>
#include <sup/gui/settings/app_settings_storage.h>
#include <sup/gui/settings/i_settings_manager.h>

#include <gtest/gtest.h>

#include <QAction>
#include <QMenu>
#include <QMenuBar>

namespace sup::gui::test
{

/**
 * @brief Tests for AnyValueEditorAppContext class.
 */
class AnyValueEditorAppContextTest : public ::testing::Test
{
};

TEST_F(AnyValueEditorAppContextTest, InitialState)
{
  AnyValueEditorAppContext context;

  EXPECT_NE(dynamic_cast<AppCommandService*>(&context.GetCommandService()), nullptr);

  auto& settings_manager = context.GetSettingsManager();
  EXPECT_NE(
      dynamic_cast<AppSettingsStorage*>(settings_manager.GetStorage(SettingsScope::kApplication)),
      nullptr);
  EXPECT_EQ(settings_manager.GetStorage(SettingsScope::kProject), nullptr);
}

}  // namespace sup::gui::test
