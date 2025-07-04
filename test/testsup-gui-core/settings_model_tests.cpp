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

#include "sup/gui/model/settings_model.h"

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

namespace sup::gui::test
{

/**
 * @brief Tests for SettingsModel class.
 */
class SettingsModelTest : public ::testing::Test
{
};

TEST_F(SettingsModelTest, InitialState)
{
  const SettingsModel model;

  EXPECT_EQ(model.GetSettingsItems().size(), 1);

  EXPECT_EQ(model.Data<bool>(constants::kUseUndoSetting), constants::kUseUndoDefault);
  EXPECT_EQ(model.Data<int>(constants::kUndoLimitSetting), constants::kUndoLimitDefault);
}

TEST_F(SettingsModelTest, Clear)
{
  SettingsModel model;

  model.GetSettingsItems().at(0)->SetProperty(constants::kUndoLimitSetting, 42);
  EXPECT_EQ(model.Data<int>(constants::kUndoLimitSetting), 42);
  model.Clear();

  // after clearing all setting items, and their values have been re-initialised
  ASSERT_EQ(model.GetSettingsItems().size(), 1);
  EXPECT_EQ(model.Data<bool>(constants::kUseUndoSetting), constants::kUseUndoDefault);
  EXPECT_EQ(model.Data<int>(constants::kUndoLimitSetting), constants::kUndoLimitDefault);
}

TEST_F(SettingsModelTest, AutoDisableSettings)
{
  SettingsModel model;

  auto common_settings = model.GetSettingsItems().at(0);
  ASSERT_EQ(model.GetSettingsItems().size(), 1);

  EXPECT_TRUE(common_settings->GetItem(constants::kUseUndoSetting)->IsEnabled());
  EXPECT_TRUE(common_settings->GetItem(constants::kUndoLimitSetting)->IsEnabled());

  // changing the value of the setting
  common_settings->GetItem(constants::kUseUndoSetting)->SetData(false);

  EXPECT_TRUE(common_settings->GetItem(constants::kUseUndoSetting)->IsEnabled());
  EXPECT_FALSE(common_settings->GetItem(constants::kUndoLimitSetting)->IsEnabled());
}

}  // namespace sup::gui::test
