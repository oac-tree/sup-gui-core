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

#include "sup/gui/views/waveformeditor/waveform_editor.h"

#include <sup/gui/views/waveformeditor/waveform_editor_widget.h>
#include <sup/gui/widgets/settings_splitter.h>

#include <gtest/gtest.h>
#include <testutils/test_settings_manager.h>

namespace sup::gui::test
{

/**
 * @brief Tests for WaveformEditor class.
 */
class WaveformEditorTest : public ::testing::Test
{
public:
  TestSettingsManager m_settings_manager;
};

TEST_F(WaveformEditorTest, InitialState)
{
  const WaveformEditor widget(m_settings_manager);
  auto waveform = widget.GetWaveform();
  EXPECT_TRUE(waveform.empty());
}

TEST_F(WaveformEditorTest, SetGetWaveform)
{
  const std::vector<std::pair<double, double>> expected = {{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}};

  WaveformEditor widget(m_settings_manager);
  widget.SetWaveform(expected, "abc");
  auto waveform = widget.GetWaveform();
  EXPECT_EQ(waveform, expected);
}

TEST_F(WaveformEditorTest, WidgetWithSettingsManager)
{
  TestSettingsManager settings_manager;
  const WaveformEditorWidget widget(settings_manager);

  EXPECT_NE(widget.findChild<SettingsSplitter*>(), nullptr);
}

}  // namespace sup::gui::test
