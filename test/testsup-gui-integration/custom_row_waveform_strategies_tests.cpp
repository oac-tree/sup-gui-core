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

#include "sup/gui/plotting/custom_row_waveform_strategies.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/plotting/waveform_helper.h>

#include <mvvm/viewmodelbase/viewitem.h>

#include <gtest/gtest.h>

using namespace sup::gui;

class CustomRowStategiesTest : public ::testing::Test
{
};

TEST_F(CustomRowStategiesTest, WaveformChildrenStrategy)
{
  TwoColumRowStrategy strategy;

  // create AnyValueItem representing a waveform
  const auto anyvalue_array_item =
      sup::gui::CreatePlotData({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});
  const auto struct_items = anyvalue_array_item->GetChildren();

  // strategy reports placeholder row for array item itself
  auto view_items = strategy.ConstructRow(anyvalue_array_item.get());
  ASSERT_EQ(view_items.size(), 2);

  // for struct at index#0 it should report row of two elements
  view_items = strategy.ConstructRow(struct_items.at(0));
  ASSERT_EQ(view_items.size(), 2);

  // x,y values should be as for the first point of the time series
  EXPECT_EQ(view_items.at(0)->Data(Qt::DisplayRole).toDouble(), 1.0);
  EXPECT_EQ(view_items.at(0)->Data(Qt::EditRole).toDouble(), 1.0);
  EXPECT_EQ(view_items.at(1)->Data(Qt::DisplayRole).toDouble(), 10.0);
  EXPECT_EQ(view_items.at(1)->Data(Qt::EditRole).toDouble(), 10.0);
}
