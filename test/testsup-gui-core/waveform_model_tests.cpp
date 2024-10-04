/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sup/gui/model/waveform_model.h"

#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/line_series_item.h>

#include <gtest/gtest.h>

using namespace sup::gui;

/**
 * @brief Tests for WaveformModel class.
 */
class WaveformModelTest : public ::testing::Test
{
};

TEST_F(WaveformModelTest, InitialState)
{
  WaveformModel model;

  ASSERT_NE(model.GetViewPort(), nullptr);
  ASSERT_NE(model.GetDataContainer(), nullptr);

  EXPECT_EQ(model.GetViewPort()->GetLineSeriesCount(), 0);
  EXPECT_EQ(model.GetDataContainer()->GetSize(), 0);

  model.InsertItem<mvvm::LineSeriesItem>(model.GetViewPort(), mvvm::TagIndex::Append());
  EXPECT_EQ(model.GetViewPort()->GetLineSeriesCount(), 1);

  model.Clear();

  EXPECT_EQ(model.GetViewPort()->GetLineSeriesCount(), 0);
  EXPECT_EQ(model.GetDataContainer()->GetSize(), 0);
}
