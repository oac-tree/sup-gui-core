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

#include "sup/gui/model/settings_item.h"

#include <sup/gui/model/settings_constants.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

using namespace sup::gui;

/**
 * @brief Tests for SettingsItem class.
 */
class SettingsItemTest : public testing::Test
{
};

//! We validate that changing a boolean stored in "Enable undo/redo" setting will
//! automatically mark "Undo limit" setting as disabled.
TEST_F(SettingsItemTest, CommonSettingsItemActivate)
{
  mvvm::ApplicationModel model;

  auto item = model.InsertItem<CommonSettingsItem>();

  EXPECT_TRUE(item->GetItem(constants::kUseUndoSetting)->IsEnabled());
  EXPECT_TRUE(item->GetItem(constants::kUndoLimitSetting)->IsEnabled());

  // changing the value of the setting
  item->GetItem(constants::kUseUndoSetting)->SetData(false);

  EXPECT_TRUE(item->GetItem(constants::kUseUndoSetting)->IsEnabled());
  EXPECT_FALSE(item->GetItem(constants::kUndoLimitSetting)->IsEnabled());
}
