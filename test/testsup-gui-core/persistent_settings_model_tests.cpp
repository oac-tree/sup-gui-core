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

#include "sup/gui/model/persistent_settings_model.h"

#include <sup/gui/model/undo_settings_item.h>

#include <mvvm/model/model_utils.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for PersistentSettingsModel class.
 */
class PersistentSettingsModelTest : public ::testing::Test
{
};

TEST_F(PersistentSettingsModelTest, InitialState)
{
  const PersistentSettingsModel model;

  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 1);

  auto undo_settings = mvvm::utils::GetTopItem<UndoSettingsItem>(model);
  ASSERT_NE(undo_settings, nullptr);
  EXPECT_EQ(undo_settings->GetUndoInfo().is_enabled, constants::kUseUndoDefault);
  EXPECT_EQ(undo_settings->GetUndoInfo().undo_limit, constants::kUndoLimitDefault);
}

TEST_F(PersistentSettingsModelTest, Clear)
{
  PersistentSettingsModel model;

  mvvm::utils::GetTopItem<UndoSettingsItem>(model)->SetUndoInfo(UndoInfo{false, 42});
  model.Clear();

  // after clearing all setting items, and their values have been re-initialised
  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 1);
  const auto undo_info = mvvm::utils::GetTopItem<UndoSettingsItem>(model)->GetUndoInfo();
  EXPECT_EQ(undo_info.is_enabled, constants::kUseUndoDefault);
  EXPECT_EQ(undo_info.undo_limit, constants::kUndoLimitDefault);
}

TEST_F(PersistentSettingsModelTest, Clone)
{
  PersistentSettingsModel model;
  mvvm::utils::GetTopItem<UndoSettingsItem>(model)->SetUndoInfo(UndoInfo{false, 42});

  auto clone = model.Clone();
  auto cloned_model = dynamic_cast<PersistentSettingsModel*>(clone.get());
  ASSERT_NE(cloned_model, nullptr);
  EXPECT_EQ(cloned_model->GetRootItem()->GetTotalItemCount(), 1);
  EXPECT_EQ(mvvm::utils::GetTopItem<UndoSettingsItem>(*cloned_model)->GetUndoInfo().undo_limit, 42);
}

}  // namespace sup::gui::test
