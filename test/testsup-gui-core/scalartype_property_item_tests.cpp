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

#include "sup/gui/model/scalartype_property_item.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <sup/dto/anytype.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

class ScalarTypePropertyItemTest : public ::testing::Test
{
};

TEST_F(ScalarTypePropertyItemTest, InitialState)
{
  ScalarTypePropertyItem item;
  EXPECT_EQ(item.GetScalarTypeName(), std::string());
}

TEST_F(ScalarTypePropertyItemTest, SetScalarTypeName)
{
  ScalarTypePropertyItem item;

  item.SetScalarTypeName(sup::dto::kInt32TypeName);
  EXPECT_EQ(item.GetScalarTypeName(), sup::dto::kInt32TypeName);

  item.SetScalarTypeName(sup::dto::kBooleanTypeName);
  EXPECT_EQ(item.GetScalarTypeName(), sup::dto::kBooleanTypeName);

  EXPECT_THROW(item.SetScalarTypeName("abc"), mvvm::RuntimeException);
}

}  // namespace sup::gui::test
