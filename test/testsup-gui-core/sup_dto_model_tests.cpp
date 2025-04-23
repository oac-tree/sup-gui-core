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

#include "sup/gui/model/sup_dto_model.h"

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

using namespace sup::gui;

/**
 * @brief Tests for SupDtoModel class.
 */
class SupDtoModelTest : public ::testing::Test
{
};

TEST_F(SupDtoModelTest, InitialState)
{
  SupDtoModel model;

  ASSERT_EQ(model.GetContainers().size(), 1);
  EXPECT_TRUE(model.GetContainers().at(0)->IsEmpty());

  model.InsertItem<mvvm::SessionItem>(model.GetContainers().at(0), mvvm::TagIndex::Append());
  EXPECT_FALSE(model.GetContainers().at(0)->IsEmpty());

  // after clear one container should remain
  model.Clear();
  ASSERT_EQ(model.GetContainers().size(), 1);
  EXPECT_TRUE(model.GetContainers().at(0)->IsEmpty());
}
