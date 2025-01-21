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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sup/gui/model/settings_model.h"

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

using namespace sup::gui;

//! Tests for SettingsModel class.
class SettingsModelTest : public ::testing::Test
{
};

TEST_F(SettingsModelTest, InitialState)
{
  SettingsModel model;

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
  EXPECT_EQ(model.GetSettingsItems().size(), 1);
  EXPECT_EQ(model.Data<bool>(constants::kUseUndoSetting), constants::kUseUndoDefault);
  EXPECT_EQ(model.Data<int>(constants::kUndoLimitSetting), constants::kUndoLimitDefault);
}
