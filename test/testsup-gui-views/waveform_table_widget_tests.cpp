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

#include "sup/gui/views/waveformeditor/waveform_table_widget.h"

#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for WaveformTableWidget class.
 */
class WaveformTableWidgetTest : public ::testing::Test
{
};

TEST_F(WaveformTableWidgetTest, GetSelectedPoint)
{
  WaveformTableWidget widget(nullptr);

  EXPECT_NO_FATAL_FAILURE(widget.SetSelectedPoint(nullptr));
}

}  // namespace sup::gui::test
