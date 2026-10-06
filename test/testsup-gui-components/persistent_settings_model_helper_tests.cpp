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

#include "sup/gui/settings/persistent_settings_model_helper.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/model/persistent_settings_model.h>
#include <sup/gui/model/undo_settings_item.h>
#include <sup/gui/settings/in_memory_settings_storage.h>
#include <sup/gui/settings/settings_manager.h>

#include <mvvm/model/model_utils.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for helper functions connecting PersistentSettingsModel and the settings manager.
 */
class PersistentSettingsModelHelperTest : public ::testing::Test
{
public:
  PersistentSettingsModelHelperTest()
  {
    m_manager.SetStorage(SettingsScope::kApplication, &m_storage);
  }

  //! Returns undo settings of the model.
  static UndoInfo GetUndoInfo(const PersistentSettingsModel& model)
  {
    return mvvm::utils::GetTopItem<UndoSettingsItem>(model)->GetUndoInfo();
  }

  InMemorySettingsStorage m_storage;
  SettingsManager m_manager;
};

TEST_F(PersistentSettingsModelHelperTest, CreateFromEmptyStorage)
{
  auto model = CreatePersistentSettingsModel(m_manager);

  const auto undo_info = GetUndoInfo(*model);
  EXPECT_EQ(undo_info.is_enabled, constants::kUseUndoDefault);
  EXPECT_EQ(undo_info.undo_limit, constants::kUndoLimitDefault);
}

TEST_F(PersistentSettingsModelHelperTest, CreateFromStoredSettings)
{
  m_manager.Set(kUndoSettingsGroup, UndoInfo{false, 42}, SettingsScope::kApplication);

  auto model = CreatePersistentSettingsModel(m_manager);

  const auto undo_info = GetUndoInfo(*model);
  EXPECT_FALSE(undo_info.is_enabled);
  EXPECT_EQ(undo_info.undo_limit, 42);
}

TEST_F(PersistentSettingsModelHelperTest, CreateWithoutStorage)
{
  const SettingsManager manager;
  EXPECT_THROW(CreatePersistentSettingsModel(manager), RuntimeException);
}

TEST_F(PersistentSettingsModelHelperTest, Write)
{
  PersistentSettingsModel model;
  mvvm::utils::GetTopItem<UndoSettingsItem>(model)->SetUndoInfo(UndoInfo{false, 42});

  WritePersistentSettingsModel(model, m_manager);

  const auto undo_info = m_manager.Get<UndoInfo>(kUndoSettingsGroup, SettingsScope::kApplication);
  EXPECT_FALSE(undo_info.is_enabled);
  EXPECT_EQ(undo_info.undo_limit, 42);
}

TEST_F(PersistentSettingsModelHelperTest, WriteNotifiesSubscribers)
{
  ::testing::MockFunction<void(const UndoInfo&)> undo_callback;
  auto connection = m_manager.Subscribe<UndoInfo>(kUndoSettingsGroup, SettingsScope::kApplication,
                                                  undo_callback.AsStdFunction());

  EXPECT_CALL(undo_callback, Call(::testing::_)).Times(1);

  WritePersistentSettingsModel(PersistentSettingsModel{}, m_manager);
}

}  // namespace sup::gui::test
