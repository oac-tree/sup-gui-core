/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Rafael Siva (EXT)
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

#include "sup/gui/widgets/window_info.h"

#include <sup/gui/settings/in_memory_settings_storage.h>
#include <sup/gui/settings/settings_archive.h>

#include <gtest/gtest.h>

#include <QWidget>

namespace sup::gui::test
{

/**
 * @brief Tests for WindowInfo struct.
 */
class WindowInfoTest : public ::testing::Test
{
public:
  InMemorySettingsStorage m_storage;
};

TEST_F(WindowInfoTest, ReadFromEmptyStorage)
{
  EXPECT_TRUE(ReadFromStorage<WindowInfo>(m_storage, "MainWindow").geometry.isEmpty());
}

TEST_F(WindowInfoTest, RoundTrip)
{
  QWidget widget;
  widget.resize(320, 240);
  const WindowInfo info{widget.saveGeometry()};

  WriteToStorage(m_storage, "MainWindow", info);

  const std::vector<std::string> expected_keys({"MainWindow/geometry"});
  EXPECT_EQ(m_storage.GetKeys(), expected_keys);
  EXPECT_EQ(ReadFromStorage<WindowInfo>(m_storage, "MainWindow").geometry, info.geometry);
}

}  // namespace sup::gui::test
