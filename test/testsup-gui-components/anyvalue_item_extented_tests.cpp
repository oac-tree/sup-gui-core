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

#include "sup/gui/model/anyvalue_item.h"

#include <mvvm/model/application_model.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/viewmodel/property_viewmodel.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for AnyValueItem appearance in the context of various viewmodel.
 */
class AnyValueItemExtendedTest : public ::testing::Test
{
};

TEST_F(AnyValueItemExtendedTest, StructWithScalarFieldInAllItemsViewModel)
{
  // we validate, that PropertyItem corresponding to type is invisible in the context of
  // AllItemsViewModel
  mvvm::ApplicationModel model;
  auto struct_item = model.InsertItem<AnyValueStructItem>();
  auto scalar0 = struct_item->AddScalarField("field0", sup::dto::kInt32TypeName, mvvm::int32{42});

  const mvvm::AllItemsViewModel view_model(&model);
  EXPECT_EQ(view_model.rowCount(), 1);
  EXPECT_EQ(view_model.columnCount(), 2);

  auto struct_index = view_model.index(0, 0, QModelIndex());
  EXPECT_EQ(view_model.rowCount(struct_index), 1);
  EXPECT_EQ(view_model.columnCount(struct_index), 2);

  auto scalar_index = view_model.index(0, 0, struct_index);
  EXPECT_EQ(view_model.rowCount(scalar_index), 0);
  EXPECT_EQ(view_model.columnCount(scalar_index), 2);
}

}  // namespace sup::gui::test
