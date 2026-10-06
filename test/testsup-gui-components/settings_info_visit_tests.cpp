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

#include <sup/gui/model/undo_settings_item.h>
#include <sup/gui/settings/in_memory_settings_storage.h>
#include <sup/gui/settings/settings_archive.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

namespace
{

constexpr auto kGroup = "Group";

}  // namespace

/**
 * @brief Tests for Visit functions of settings structs.
 */
class SettingsInfoVisitTest : public ::testing::Test
{
public:
  InMemorySettingsStorage m_storage;
};

TEST_F(SettingsInfoVisitTest, UndoInfoReadFromEmptyStorage)
{
  const auto info = ReadFromStorage<UndoInfo>(m_storage, kGroup);

  EXPECT_EQ(info.is_enabled, constants::kUseUndoDefault);
  EXPECT_EQ(info.undo_limit, constants::kUndoLimitDefault);
}

TEST_F(SettingsInfoVisitTest, UndoInfoWriteKeys)
{
  WriteToStorage(m_storage, kGroup, UndoInfo{false, 42});

  const std::vector<std::string> expected_keys({"Group/is_enabled", "Group/undo_limit"});
  EXPECT_EQ(m_storage.GetKeys(), expected_keys);
  EXPECT_EQ(m_storage.Read("Group/is_enabled"), mvvm::variant_t{false});
  EXPECT_EQ(m_storage.Read("Group/undo_limit"), mvvm::variant_t{mvvm::int32{42}});
}

TEST_F(SettingsInfoVisitTest, UndoInfoKeysInSettingsGroup)
{
  WriteToStorage(m_storage, kUndoSettingsGroup, UndoInfo{});

  const std::vector<std::string> expected_keys(
      {"Settings/Undo/is_enabled", "Settings/Undo/undo_limit"});
  EXPECT_EQ(m_storage.GetKeys(), expected_keys);
}

TEST_F(SettingsInfoVisitTest, UndoInfoRoundTrip)
{
  WriteToStorage(m_storage, kGroup, UndoInfo{false, 42});

  const auto info = ReadFromStorage<UndoInfo>(m_storage, kGroup);

  EXPECT_FALSE(info.is_enabled);
  EXPECT_EQ(info.undo_limit, 42);
}

}  // namespace sup::gui::test
