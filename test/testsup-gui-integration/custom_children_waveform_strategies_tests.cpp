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

#include "sup/gui/plotting/custom_children_waveform_strategies.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/plotting/waveform_helper.h>

#include <mvvm/utils/container_utils.h>

#include <gtest/gtest.h>

using namespace sup::gui;

class CustomChildrenStategiesTest : public ::testing::Test
{
};

TEST_F(CustomChildrenStategiesTest, WaveformChildrenStrategy)
{
  WaveformChildrenStrategy strategy;

  // create AnyValueItem representing a waveform
  const auto anyvalue_array_item =
      sup::gui::CreatePlotData({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});
  const auto struct_items = anyvalue_array_item->GetChildren();

  // strategy report underlying AnyValueStructItem's as children of our array
  auto children = mvvm::utils::CastItems<sup::gui::AnyValueItem>(
      strategy.GetChildren(anyvalue_array_item.get()));
  ASSERT_EQ(children.size(), 3);
  EXPECT_EQ(children, struct_items);

  // strategy doesn't see x,y children below each struct
  EXPECT_TRUE(strategy.GetChildren(struct_items.at(0)).empty());
}
