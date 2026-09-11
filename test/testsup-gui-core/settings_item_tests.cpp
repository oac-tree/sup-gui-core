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

#include "sup/gui/model/common_settings_item.h"

#include <sup/gui/model/settings_constants.h>

#include <mvvm/model/application_model.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for CommonSettingsItem class.
 */
class CommonSettingsItemTest : public testing::Test
{
};

TEST_F(CommonSettingsItemTest, CommonSettingsItem)
{
  EXPECT_TRUE(mvvm::test::IsCloneImplemented<CommonSettingsItem>());

  CommonSettingsItem item;
  EXPECT_EQ(mvvm::GetTypeName<CommonSettingsItem>(), "CommonSettings");
  EXPECT_EQ(mvvm::GetTypeName<CommonSettingsItem>(), item.GetType());
  EXPECT_EQ(item.GetDisplayName(), "Common Settings");
}

//! We validate that changing a boolean stored in "Enable undo/redo" setting will
//! automatically mark "Undo limit" setting as disabled.
TEST_F(CommonSettingsItemTest, DataStrategy)
{
  CommonSettingsItem item;

  EXPECT_TRUE(item.GetItem(constants::kUseUndoSetting).IsEnabled());
  EXPECT_TRUE(item.GetItem(constants::kUndoLimitSetting).IsEnabled());

  // changing the value of the setting
  item.GetItem(constants::kUseUndoSetting).SetData(false);

  EXPECT_TRUE(item.GetItem(constants::kUseUndoSetting).IsEnabled());
  EXPECT_FALSE(item.GetItem(constants::kUndoLimitSetting).IsEnabled());
}

TEST_F(CommonSettingsItemTest, CloneItem)
{
  CommonSettingsItem item;

  auto clone = item.Clone();

  EXPECT_TRUE(clone->GetItem(constants::kUseUndoSetting).IsEnabled());
  EXPECT_TRUE(clone->GetItem(constants::kUndoLimitSetting).IsEnabled());

  // validating that data strategy is working after the clone

  clone->GetItem(constants::kUseUndoSetting).SetData(false);
  EXPECT_TRUE(clone->GetItem(constants::kUseUndoSetting).IsEnabled());
  EXPECT_FALSE(clone->GetItem(constants::kUndoLimitSetting).IsEnabled());
}

}  // namespace sup::gui::test
