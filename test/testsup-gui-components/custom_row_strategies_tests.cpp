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

#include "sup/gui/viewmodel/custom_row_strategies.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <mvvm/viewmodelbase/viewitem.h>

#include <sup/dto/anytype.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

//! Testing strategies from custom_row_strategies.h

class CustomRowStrategiesTest : public ::testing::Test
{
};

TEST_F(CustomRowStrategiesTest, ScalarItem)
{
  AnyValueScalarItem item;

  item.SetAnyTypeName(sup::dto::kInt8TypeName);
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);

  AnyValueRowStrategy strategy;
  auto view_items = strategy.ConstructRow(&item);

  ASSERT_EQ(view_items.size(), 3);

  EXPECT_EQ(view_items.at(0)->Data(Qt::DisplayRole).toString().toStdString(),
            constants::kScalarTypeName);
  EXPECT_EQ(view_items.at(0)->Data(Qt::EditRole).toString().toStdString(),
            constants::kScalarTypeName);

  EXPECT_EQ(view_items.at(1)->Data(Qt::DisplayRole).toInt(), 0);
  EXPECT_EQ(view_items.at(1)->Data(Qt::EditRole).toInt(), 0);

  EXPECT_EQ(view_items.at(2)->Data(Qt::DisplayRole).toString().toStdString(),
            sup::dto::kInt8TypeName);
  EXPECT_FALSE(view_items.at(2)->Data(Qt::EditRole).isValid());

  // more tests in AnyValueViewModelTest
}

}  // namespace sup::gui::test
