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

#include "sup/gui/model/scalar_conversion_utils.h"

#include <sup/gui/model/anyvalue_conversion_utils.h>

#include <mvvm/core/variant.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/basic_scalar_types.h>

#include <gtest/gtest.h>

using namespace sup::gui;

class ScalarConversionUtilsTest : public ::testing::Test
{
public:
  //! Helper function to test if we can correctly create default constructed variant from type name.
  template <typename T>
  bool IsValidVariantForName(const std::string& type_name)
  {
    T default_value{};
    auto variant = GetVariantFromScalarTypeName(type_name);
    bool is_valid_type = std::holds_alternative<T>(variant);
    bool is_valid_value = std::get<T>(variant) == default_value;
    return is_valid_type && is_valid_value;
  }
};

TEST_F(ScalarConversionUtilsTest, SetVariantFromScalar)
{
  {  // from bool
    sup::dto::boolean value{true};
    sup::dto::AnyValue anyvalue{sup::dto::BooleanType};
    anyvalue = value;
    auto variant = GetVariantFromScalar(anyvalue);
    EXPECT_EQ(GetTypeCode(variant), mvvm::TypeCode::Bool);
    EXPECT_EQ(variant, mvvm::variant_t(value));
  }

  {  // from char8
    sup::dto::char8 value{42};
    sup::dto::AnyValue anyvalue{sup::dto::Character8Type};
    anyvalue = value;
    auto variant = GetVariantFromScalar(anyvalue);
    EXPECT_EQ(GetTypeCode(variant), mvvm::TypeCode::Char8);
    EXPECT_EQ(variant, mvvm::variant_t(value));
  }

  {  // from int8
    sup::dto::int8 value{42};
    sup::dto::AnyValue anyvalue{sup::dto::SignedInteger8Type};
    anyvalue = value;
    auto variant = GetVariantFromScalar(anyvalue);
    EXPECT_EQ(GetTypeCode(variant), mvvm::TypeCode::Int8);
    EXPECT_EQ(variant, mvvm::variant_t(value));
  }

  {  // from uint8
    sup::dto::uint8 value{42};
    sup::dto::AnyValue anyvalue{sup::dto::UnsignedInteger8Type};
    anyvalue = value;
    auto variant = GetVariantFromScalar(anyvalue);
    EXPECT_EQ(GetTypeCode(variant), mvvm::TypeCode::UInt8);
    EXPECT_EQ(variant, mvvm::variant_t(value));
  }

  {  // from int16
    sup::dto::int16 value{42};
    sup::dto::AnyValue anyvalue{sup::dto::SignedInteger16Type};
    anyvalue = value;
    auto variant = GetVariantFromScalar(anyvalue);
    EXPECT_EQ(GetTypeCode(variant), mvvm::TypeCode::Int16);
    EXPECT_EQ(variant, mvvm::variant_t(value));
  }

  {  // from uint16
    sup::dto::uint16 value{42};
    sup::dto::AnyValue anyvalue{sup::dto::UnsignedInteger16Type};
    anyvalue = value;
    auto variant = GetVariantFromScalar(anyvalue);
    EXPECT_EQ(GetTypeCode(variant), mvvm::TypeCode::UInt16);
    EXPECT_EQ(variant, mvvm::variant_t(value));
  }

  {  // from int32
    sup::dto::int32 value{42};
    sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type};
    anyvalue = value;
    auto variant = GetVariantFromScalar(anyvalue);
    EXPECT_EQ(GetTypeCode(variant), mvvm::TypeCode::Int32);
    EXPECT_EQ(variant, mvvm::variant_t(value));
  }

  {  // from uint32
    sup::dto::uint32 value{42};
    sup::dto::AnyValue anyvalue{sup::dto::UnsignedInteger32Type};
    anyvalue = value;
    auto variant = GetVariantFromScalar(anyvalue);
    EXPECT_EQ(GetTypeCode(variant), mvvm::TypeCode::UInt32);
    EXPECT_EQ(variant, mvvm::variant_t(value));
  }

  {  // from int64
    sup::dto::int64 value{42};
    sup::dto::AnyValue anyvalue{sup::dto::SignedInteger64Type};
    anyvalue = value;
    auto variant = GetVariantFromScalar(anyvalue);
    EXPECT_EQ(GetTypeCode(variant), mvvm::TypeCode::Int64);
    EXPECT_EQ(variant, mvvm::variant_t(value));
  }

  {  // from uint64
    sup::dto::uint64 value{42};
    sup::dto::AnyValue anyvalue{sup::dto::UnsignedInteger64Type};
    anyvalue = value;
    auto variant = GetVariantFromScalar(anyvalue);
    EXPECT_EQ(GetTypeCode(variant), mvvm::TypeCode::UInt64);
    EXPECT_EQ(variant, mvvm::variant_t(value));
  }

  {  // from float32
    sup::dto::float32 value{42.1};
    sup::dto::AnyValue anyvalue{sup::dto::Float32Type};
    anyvalue = value;
    auto variant = GetVariantFromScalar(anyvalue);
    EXPECT_EQ(GetTypeCode(variant), mvvm::TypeCode::Float32);
    EXPECT_EQ(variant, mvvm::variant_t(value));
  }

  {  // from float64
    sup::dto::float64 value{42.1};
    sup::dto::AnyValue anyvalue{sup::dto::Float64Type};
    anyvalue = value;
    auto variant = GetVariantFromScalar(anyvalue);
    EXPECT_EQ(GetTypeCode(variant), mvvm::TypeCode::Float64);
    EXPECT_EQ(variant, mvvm::variant_t(value));
  }

  {  // from string
    std::string value{"abc"};
    sup::dto::AnyValue anyvalue{sup::dto::StringType};
    anyvalue = value;
    auto variant = GetVariantFromScalar(anyvalue);
    EXPECT_EQ(GetTypeCode(variant), mvvm::TypeCode::String);
    EXPECT_EQ(variant, mvvm::variant_t(value));
  }
}

//! Testing GetAnyValueFromScalar method. Creating scalar AnyValue from scalar-like variants.

TEST_F(ScalarConversionUtilsTest, GetAnyValueFromScalarVariant)
{
  {  // boolean
    mvvm::boolean value{true};
    auto any_value = GetAnyValueFromScalar(value);
    EXPECT_EQ(any_value.GetType(), sup::dto::BooleanType);
    EXPECT_EQ(any_value, true);
  }

  {  // char8
    mvvm::char8 value{42};
    auto any_value = GetAnyValueFromScalar(value);
    EXPECT_EQ(any_value.GetType(), sup::dto::Character8Type);
    EXPECT_EQ(any_value.As<mvvm::char8>(), value);
  }

  {  // int8
    mvvm::int8 value{42};
    auto any_value = GetAnyValueFromScalar(value);
    EXPECT_EQ(any_value.GetType(), sup::dto::SignedInteger8Type);
    EXPECT_EQ(any_value.As<mvvm::int8>(), value);
  }

  {  // uint8
    mvvm::uint8 value{42};
    auto any_value = GetAnyValueFromScalar(value);
    EXPECT_EQ(any_value.GetType(), sup::dto::UnsignedInteger8Type);
    EXPECT_EQ(any_value.As<mvvm::uint8>(), value);
  }

  {  // int16
    mvvm::int16 value{42};
    auto any_value = GetAnyValueFromScalar(value);
    EXPECT_EQ(any_value.GetType(), sup::dto::SignedInteger16Type);
    EXPECT_EQ(any_value.As<mvvm::int16>(), value);
  }

  {  // uint16
    mvvm::uint16 value{42};
    auto any_value = GetAnyValueFromScalar(value);
    EXPECT_EQ(any_value.GetType(), sup::dto::UnsignedInteger16Type);
    EXPECT_EQ(any_value.As<mvvm::uint16>(), value);
  }

  {  // int32
    mvvm::int32 value{42};
    auto any_value = GetAnyValueFromScalar(value);
    EXPECT_EQ(any_value.GetType(), sup::dto::SignedInteger32Type);
    EXPECT_EQ(any_value.As<mvvm::int32>(), value);
  }

  {  // uint32
    mvvm::uint32 value{42};
    auto any_value = GetAnyValueFromScalar(value);
    EXPECT_EQ(any_value.GetType(), sup::dto::UnsignedInteger32Type);
    EXPECT_EQ(any_value.As<mvvm::uint32>(), value);
  }

  {  // int64
    mvvm::int64 value{42};
    auto any_value = GetAnyValueFromScalar(value);
    EXPECT_EQ(any_value.GetType(), sup::dto::SignedInteger64Type);
    EXPECT_EQ(any_value.As<mvvm::int64>(), value);
  }

  {  // uint64
    mvvm::uint64 value{42};
    auto any_value = GetAnyValueFromScalar(value);
    EXPECT_EQ(any_value.GetType(), sup::dto::UnsignedInteger64Type);
    EXPECT_EQ(any_value.As<mvvm::uint64>(), value);
  }

  {  // float32
    mvvm::float32 value{42.1};
    auto any_value = GetAnyValueFromScalar(value);
    EXPECT_EQ(any_value.GetType(), sup::dto::Float32Type);
    EXPECT_EQ(any_value.As<mvvm::float32>(), value);
  }

  {  // float64
    mvvm::float64 value{42.1};
    auto any_value = GetAnyValueFromScalar(value);
    EXPECT_EQ(any_value.GetType(), sup::dto::Float64Type);
    EXPECT_EQ(any_value.As<mvvm::float64>(), value);
  }

  {  // string
    std::string value{"abc"};
    auto any_value = GetAnyValueFromScalar(value);
    EXPECT_EQ(any_value.GetType(), sup::dto::StringType);
    EXPECT_EQ(any_value.As<std::string>(), value);
  }
}

//! Checking function to get variant_t from sup::dto type names.

TEST_F(ScalarConversionUtilsTest, GetVariantFromScalarTypeName)
{
  EXPECT_TRUE(IsValidVariantForName<bool>(sup::dto::kBooleanTypeName));
  EXPECT_TRUE(IsValidVariantForName<mvvm::char8>(sup::dto::kChar8TypeName));
  EXPECT_TRUE(IsValidVariantForName<mvvm::int8>(sup::dto::kInt8TypeName));
  EXPECT_TRUE(IsValidVariantForName<mvvm::uint8>(sup::dto::kUInt8TypeName));
  EXPECT_TRUE(IsValidVariantForName<mvvm::int16>(sup::dto::kInt16TypeName));
  EXPECT_TRUE(IsValidVariantForName<mvvm::uint16>(sup::dto::kUInt16TypeName));
  EXPECT_TRUE(IsValidVariantForName<mvvm::int32>(sup::dto::kInt32TypeName));
  EXPECT_TRUE(IsValidVariantForName<mvvm::uint32>(sup::dto::kUInt32TypeName));
  EXPECT_TRUE(IsValidVariantForName<mvvm::int64>(sup::dto::kInt64TypeName));
  EXPECT_TRUE(IsValidVariantForName<mvvm::uint64>(sup::dto::kUInt64TypeName));
  EXPECT_TRUE(IsValidVariantForName<mvvm::float32>(sup::dto::kFloat32TypeName));
  EXPECT_TRUE(IsValidVariantForName<mvvm::float64>(sup::dto::kFloat64TypeName));
  EXPECT_TRUE(IsValidVariantForName<std::string>(sup::dto::kStringTypeName));
}
