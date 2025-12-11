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

#include "sup/gui/model/anyvalue_item_constants.h"

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

class AnyValueItemConstantsTest : public ::testing::Test
{
};

TEST_F(AnyValueItemConstantsTest, CompareWithDomainConstants)
{
  EXPECT_EQ(constants::kEmptyTypeName, sup::dto::kEmptyTypeName);
  EXPECT_EQ(constants::kBooleanTypeName, sup::dto::kBooleanTypeName);
  EXPECT_EQ(constants::kChar8TypeName, sup::dto::kChar8TypeName);
  EXPECT_EQ(constants::kInt8TypeName, sup::dto::kInt8TypeName);
  EXPECT_EQ(constants::kUInt8TypeName, sup::dto::kUInt8TypeName);
  EXPECT_EQ(constants::kInt16TypeName, sup::dto::kInt16TypeName);
  EXPECT_EQ(constants::kUInt16TypeName, sup::dto::kUInt16TypeName);
  EXPECT_EQ(constants::kInt32TypeName, sup::dto::kInt32TypeName);
  EXPECT_EQ(constants::kUInt32TypeName, sup::dto::kUInt32TypeName);
  EXPECT_EQ(constants::kInt64TypeName, sup::dto::kInt64TypeName);
  EXPECT_EQ(constants::kUInt64TypeName, sup::dto::kUInt64TypeName);
  EXPECT_EQ(constants::kFloat32TypeName, sup::dto::kFloat32TypeName);
  EXPECT_EQ(constants::kFloat64TypeName, sup::dto::kFloat64TypeName);
  EXPECT_EQ(constants::kStringTypeName, sup::dto::kStringTypeName);
}

}  // namespace sup::gui::test
