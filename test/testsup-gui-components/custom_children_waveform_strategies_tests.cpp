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

#include "sup/gui/plotting/custom_children_waveform_strategies.h"

#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/point_item.h>
#include <mvvm/utils/container_utils.h>

#include <gtest/gtest.h>

using namespace sup::gui;

//! Tests for WaveformChildrenStrategy class.

class CustomChildrenStategiesTest : public ::testing::Test
{
};

TEST_F(CustomChildrenStategiesTest, WaveformChildrenStrategy)
{
  WaveformChildrenStrategy strategy;

  mvvm::LineSeriesDataItem data_item;
  data_item.SetWaveform({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});

  auto points = data_item.GetPoints();

  // strategy report underlying points as children of LineSeriesDataItem
  auto children = mvvm::utils::CastItems<mvvm::PointItem>(strategy.GetChildren(&data_item));
  ASSERT_EQ(children.size(), 3);
  EXPECT_EQ(children, points);

  // strategy doesn't see x,y below each point
  EXPECT_TRUE(strategy.GetChildren(points.at(0)).empty());
}
