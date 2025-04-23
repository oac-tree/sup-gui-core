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

#include "sup/gui/model/anyvalue_conversion_utils.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>
#include <sup/gui/model/scalar_conversion_utils.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

using namespace sup::gui;

class AnyValueConversionUtilsTest : public ::testing::Test
{
};

TEST_F(AnyValueConversionUtilsTest, GetTypeCode)
{
  using sup::dto::TypeCode;

  EXPECT_EQ(GetScalarTypeCode(sup::dto::kBooleanTypeName), TypeCode::Bool);
  EXPECT_EQ(GetScalarTypeCode(sup::dto::kChar8TypeName), TypeCode::Char8);
  EXPECT_EQ(GetScalarTypeCode(sup::dto::kInt8TypeName), TypeCode::Int8);
  EXPECT_EQ(GetScalarTypeCode(sup::dto::kUInt8TypeName), TypeCode::UInt8);
  EXPECT_EQ(GetScalarTypeCode(sup::dto::kInt16TypeName), TypeCode::Int16);
  EXPECT_EQ(GetScalarTypeCode(sup::dto::kUInt16TypeName), TypeCode::UInt16);
  EXPECT_EQ(GetScalarTypeCode(sup::dto::kInt32TypeName), TypeCode::Int32);
  EXPECT_EQ(GetScalarTypeCode(sup::dto::kUInt32TypeName), TypeCode::UInt32);
  EXPECT_EQ(GetScalarTypeCode(sup::dto::kInt64TypeName), TypeCode::Int64);
  EXPECT_EQ(GetScalarTypeCode(sup::dto::kUInt64TypeName), TypeCode::UInt64);
  EXPECT_EQ(GetScalarTypeCode(sup::dto::kFloat32TypeName), TypeCode::Float32);
  EXPECT_EQ(GetScalarTypeCode(sup::dto::kFloat64TypeName), TypeCode::Float64);
  EXPECT_EQ(GetScalarTypeCode(sup::dto::kStringTypeName), TypeCode::String);
  EXPECT_THROW(GetScalarTypeCode(constants::kStructTypeName), RuntimeException);
}

//! Testing IsScalarTypeName utility function.

TEST_F(AnyValueConversionUtilsTest, IsScalarTypeName)
{
  EXPECT_FALSE(IsScalarTypeName(std::string("")));

  EXPECT_FALSE(IsScalarTypeName(constants::kStructTypeName));
  EXPECT_FALSE(IsScalarTypeName(constants::kArrayTypeName));
  EXPECT_FALSE(IsScalarTypeName(constants::kScalarTypeName));

  EXPECT_TRUE(IsScalarTypeName(sup::dto::kInt8TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kUInt8TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kInt16TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kUInt16TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kInt32TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kUInt32TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kInt64TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kUInt64TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kFloat32TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kFloat64TypeName));
  EXPECT_TRUE(IsScalarTypeName(sup::dto::kStringTypeName));
}

//! Testing IsStructTypeName utility function.

TEST_F(AnyValueConversionUtilsTest, IsStructTypeName)
{
  EXPECT_FALSE(IsStructTypeName(std::string("")));

  EXPECT_TRUE(IsStructTypeName(constants::kStructTypeName));
  EXPECT_FALSE(IsStructTypeName(constants::kArrayTypeName));
  EXPECT_FALSE(IsStructTypeName(constants::kScalarTypeName));

  EXPECT_FALSE(IsStructTypeName(sup::dto::kInt8TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kUInt8TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kInt16TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kUInt16TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kInt32TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kUInt32TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kInt64TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kUInt64TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kFloat32TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kFloat64TypeName));
  EXPECT_FALSE(IsStructTypeName(sup::dto::kStringTypeName));
}

//! Testing SetDataFromScalar method.

TEST_F(AnyValueConversionUtilsTest, SetDataFromScalar)
{
  {  // from bool
    sup::dto::AnyValue anyvalue{sup::dto::BooleanType};
    anyvalue = true;
    AnyValueScalarItem item;
    SetDataFromScalar(anyvalue, item);
    EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kBooleanTypeName);
    EXPECT_EQ(item.GetAnyTypeName(), anyvalue.GetTypeName());
    EXPECT_TRUE(item.Data<bool>());
    EXPECT_EQ(item.GetTotalItemCount(), 0);
  }

  {  // from int
    sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type};
    anyvalue.ConvertFrom(42);
    AnyValueScalarItem item;
    SetDataFromScalar(anyvalue, item);
    EXPECT_EQ(item.GetAnyTypeName(), anyvalue.GetTypeName());
    EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kInt32TypeName);
    EXPECT_EQ(item.Data<int>(), 42);
    EXPECT_EQ(item.GetTotalItemCount(), 0);
  }

  {  // from uint
    sup::dto::AnyValue anyvalue{sup::dto::UnsignedInteger32Type};
    anyvalue.ConvertFrom(42);
    AnyValueScalarItem item;
    SetDataFromScalar(anyvalue, item);
    EXPECT_EQ(item.GetAnyTypeName(), anyvalue.GetTypeName());
    EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kUInt32TypeName);
    EXPECT_EQ(item.Data<mvvm::uint32>(), 42);
    EXPECT_EQ(item.GetTotalItemCount(), 0);
  }

  {  // changing uint to bool
    sup::dto::AnyValue anyvalue{sup::dto::UnsignedInteger32Type};
    anyvalue.ConvertFrom(42);

    AnyValueScalarItem item;
    SetDataFromScalar(anyvalue, item);
    EXPECT_EQ(item.GetAnyTypeName(), anyvalue.GetTypeName());
    EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kUInt32TypeName);
    EXPECT_EQ(item.Data<mvvm::uint32>(), 42);
    EXPECT_EQ(item.GetTotalItemCount(), 0);

    sup::dto::AnyValue new_anyvalue{sup::dto::BooleanType};
    new_anyvalue = true;
    SetDataFromScalar(new_anyvalue, item);
    EXPECT_NE(item.GetAnyTypeName(), anyvalue.GetTypeName());
    EXPECT_EQ(item.GetAnyTypeName(), new_anyvalue.GetTypeName());
    EXPECT_EQ(mvvm::utils::TypeName(item.Data()), mvvm::constants::kBooleanTypeName);
    EXPECT_EQ(item.Data<bool>(), true);
    EXPECT_EQ(item.GetTotalItemCount(), 0);
  }
}

//! Testing GetAnyValueFromScalar method. Creating scalar AnyValue from scalar-like AnyValueItem
//! containing various scalars.

TEST_F(AnyValueConversionUtilsTest, GetAnyValueFromScalar)
{
  {  // boolean
    AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kBooleanTypeName);
    item.SetData(true);

    auto any_value = GetAnyValueFromScalar(item);
    EXPECT_EQ(any_value.GetType(), sup::dto::BooleanType);
    EXPECT_EQ(any_value, true);
  }

  {  // int32
    AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kInt32TypeName);
    item.SetData(42);

    auto any_value = GetAnyValueFromScalar(item);
    EXPECT_EQ(any_value.GetType(), sup::dto::SignedInteger32Type);
    EXPECT_EQ(any_value, 42);
  }

  {  // uint32
    AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kUInt32TypeName);
    item.SetData(42U);

    auto any_value = GetAnyValueFromScalar(item);
    EXPECT_EQ(any_value.GetType(), sup::dto::UnsignedInteger32Type);
    EXPECT_EQ(any_value, 42U);
  }

  {  // double
    AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kFloat64TypeName);
    item.SetData(42.1);

    auto any_value = GetAnyValueFromScalar(item);
    EXPECT_EQ(any_value.GetType(), sup::dto::Float64Type);
    EXPECT_EQ(any_value, 42.1);
  }

  {  // attempt to construct a scalar from the struct
    const AnyValueStructItem item;
    EXPECT_THROW(GetAnyValueFromScalar(item), RuntimeException);
  }
}
