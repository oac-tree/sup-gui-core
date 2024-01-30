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

#include "sup/gui/viewmodel/custom_children_strategies.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <mvvm/standarditems/container_item.h>

#include <sup/dto/anytype.h>

#include <gtest/gtest.h>

using namespace sup::gui;

//! Testing strategies from custom_children_strategies.h

class CustomChildrenStrategiesTest : public ::testing::Test
{
};

TEST_F(CustomChildrenStrategiesTest, ItemInContainer)
{
  mvvm::ContainerItem container;

  AnyValueChildrenStrategy strategy;
  EXPECT_TRUE(strategy.GetChildren(&container).empty());

  auto scalar_item = container.InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  EXPECT_EQ(strategy.GetChildren(&container), std::vector<mvvm::SessionItem*>({scalar_item}));

  EXPECT_TRUE(strategy.GetChildren(scalar_item).empty());

  // more tests in AnyValueViewModelTest
}
