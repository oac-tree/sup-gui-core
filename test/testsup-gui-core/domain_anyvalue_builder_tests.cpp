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

#include "sup/gui/model/domain_anyvalue_builder.h"

#include <sup/gui/model/anyvalue_item.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

class DomainAnyValueBuilderTest : public ::testing::Test
{
public:
  static sup::dto::AnyValue CreateAnyValue(const AnyValueItem& item)
  {
    DomainAnyValueBuilder builder(item);
    return builder.GetAnyValue();
  }
};

//! Build from empty AnyValueItem.

TEST_F(DomainAnyValueBuilderTest, EmptyValue)
{
  const AnyValueItem item("test");

  auto any_value = CreateAnyValue(item);
  EXPECT_TRUE(sup::dto::IsEmptyValue(any_value));
}

//! Build AnyValue from AnyValueItem representing a scalar.
//! More similar tests in ScalarConversionUtilsTests::GetAnyValueFromScalar.

TEST_F(DomainAnyValueBuilderTest, FromScalar)
{
  {  // from bool
    AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kBooleanTypeName);
    item.SetData(true);
    const sup::dto::AnyValue expected_anyvalue{sup::dto::BooleanType, true};
    auto any_value = CreateAnyValue(item);
    EXPECT_EQ(any_value, expected_anyvalue);
  }

  {  // from int32
    AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kInt32TypeName);
    item.SetData(42);
    const sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 42};
    auto any_value = CreateAnyValue(item);
    EXPECT_EQ(any_value, expected_anyvalue);
  }

  {
    AnyValueScalarItem item;
    item.SetAnyTypeName(sup::dto::kInt32TypeName);
    item.SetData(42);
    const sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 42};
    auto any_value = CreateAnyValue(item);
    EXPECT_EQ(any_value, expected_anyvalue);
  }
}

//! real-life bug
TEST_F(DomainAnyValueBuilderTest, FromScalarAndThenChangeScalarType)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(sup::dto::kInt32TypeName);
  item.SetData(42);
  const sup::dto::AnyValue expected_anyvalue1{sup::dto::SignedInteger32Type, 42};
  auto any_value1 = CreateAnyValue(item);
  EXPECT_EQ(any_value1, expected_anyvalue1);

  item.SetAnyTypeName(sup::dto::kBooleanTypeName);
  const sup::dto::AnyValue expected_anyvalue2{sup::dto::BooleanType, false};
  auto any_value2 = CreateAnyValue(item);
  EXPECT_EQ(any_value2, expected_anyvalue2);
}

TEST_F(DomainAnyValueBuilderTest, FromEmptyStruct)
{
  {  // empty struct
    const AnyValueStructItem item;
    const sup::dto::AnyValue expected_anyvalue{::sup::dto::EmptyStruct()};
    auto any_value = CreateAnyValue(item);
    EXPECT_EQ(any_value, expected_anyvalue);
  }

  {  // empty named struct
    AnyValueStructItem item;
    item.SetAnyTypeName("my_type");
    const sup::dto::AnyValue expected_anyvalue{::sup::dto::EmptyStruct("my_type")};
    auto any_value = CreateAnyValue(item);
    EXPECT_EQ(any_value, expected_anyvalue);
  }
}

TEST_F(DomainAnyValueBuilderTest, FromStructWithSingleScalar)
{
  AnyValueStructItem item;
  item.AddScalarField("signed", sup::dto::kInt32TypeName, 42);

  const sup::dto::AnyValue expected_anyvalue = {{{"signed", {sup::dto::SignedInteger32Type, 42}}}};
  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected_anyvalue);
}

TEST_F(DomainAnyValueBuilderTest, FromStructWithTwoScalars)
{
  AnyValueStructItem item;
  item.SetAnyTypeName("my_struct");
  item.AddScalarField("signed", sup::dto::kInt32TypeName, 42);
  item.AddScalarField("bool", sup::dto::kBooleanTypeName, true);

  const sup::dto::AnyValue expected_anyvalue = {
      {{"signed", {sup::dto::SignedInteger32Type, 42}}, {"bool", {sup::dto::BooleanType, true}}},
      "my_struct"};

  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected_anyvalue);
}

TEST_F(DomainAnyValueBuilderTest, FromNestedStruct)
{
  AnyValueStructItem item;
  auto inner_struct = item.InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  inner_struct->SetDisplayName("scalars");
  const mvvm::int8 num(1);
  inner_struct->AddScalarField("signed", sup::dto::kInt8TypeName, num);
  inner_struct->AddScalarField("bool", sup::dto::kBooleanTypeName, true);

  sup::dto::AnyValue two_scalars = {
      {{"signed", {sup::dto::SignedInteger8Type, 1}}, {"bool", {sup::dto::BooleanType, 12}}}};
  const sup::dto::AnyValue expected_anyvalue{{
      {"scalars", two_scalars},
  }};

  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected_anyvalue);
}

TEST_F(DomainAnyValueBuilderTest, FromTwoNestedStructs)
{
  AnyValueStructItem item;
  item.SetAnyTypeName("nested_struct");

  auto inner_struct0 = item.InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  inner_struct0->SetDisplayName("struct0");
  const mvvm::int8 num1(-1);
  const mvvm::uint8 num2(1);
  inner_struct0->AddScalarField("signed", sup::dto::kInt8TypeName, num1);
  inner_struct0->AddScalarField("unsigned", sup::dto::kUInt8TypeName, num2);

  auto inner_struct1 = item.InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  inner_struct1->SetDisplayName("struct1");
  const mvvm::int8 num3(0);
  const mvvm::int8 num4(5);
  inner_struct1->AddScalarField("first", sup::dto::kInt8TypeName, num3);
  inner_struct1->AddScalarField("second", sup::dto::kInt8TypeName, num4);

  const std::string nested_name = "nested_struct";
  sup::dto::AnyValue two_scalars = {{{"signed", {sup::dto::SignedInteger8Type, num1}},
                                     {"unsigned", {sup::dto::UnsignedInteger8Type, num2}}}};
  const sup::dto::AnyValue expected_anyvalue{{{"struct0", two_scalars},
                                              {"struct1",
                                               {{"first", {sup::dto::SignedInteger8Type, num3}},
                                                {"second", {sup::dto::SignedInteger8Type, num4}}}}},
                                             nested_name};
  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected_anyvalue);
}

//! Construction of empty scalar array.

TEST_F(DomainAnyValueBuilderTest, FromEmptyScalarArray)
{
  AnyValueArrayItem item;
  item.SetAnyTypeName("array_name");

  EXPECT_THROW(CreateAnyValue(item), sup::dto::ParseException);
}

//! Construction of scalar array with the name and two elements.

TEST_F(DomainAnyValueBuilderTest, FromScalarArray)
{
  auto expected = sup::dto::ArrayValue({{sup::dto::SignedInteger32Type, 42}, 43}, "array_name");

  AnyValueArrayItem item;
  item.SetAnyTypeName("array_name");

  auto child0 = item.InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  child0->SetAnyTypeName(sup::dto::kInt32TypeName);
  child0->SetData(42);

  auto child1 = item.InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  child1->SetAnyTypeName(sup::dto::kInt32TypeName);
  child1->SetData(43);

  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected);
}

//! Building a structure with scalar array as a single field.

TEST_F(DomainAnyValueBuilderTest, StructWithScalarArrayAsField)
{
  auto array_value = sup::dto::ArrayValue({{sup::dto::SignedInteger32Type, 42}, 43}, "array_name");
  const sup::dto::AnyValue expected_struct_value = {{{"array_field", array_value}}, "struct_name"};

  AnyValueStructItem item;
  item.SetAnyTypeName("struct_name");

  auto array = item.InsertItem<AnyValueArrayItem>(mvvm::TagIndex::Append());
  array->SetAnyTypeName("array_name");
  array->SetDisplayName("array_field");

  auto element0 = array->InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  element0->SetAnyTypeName(sup::dto::kInt32TypeName);
  element0->SetData(42);

  auto element1 = array->InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  element1->SetAnyTypeName(sup::dto::kInt32TypeName);
  element1->SetData(43);

  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected_struct_value);
}

//! Building a structure with two scalar array as fields.

TEST_F(DomainAnyValueBuilderTest, StructWithTwoScalarArrayAsField)
{
  auto array_value1 =
      sup::dto::ArrayValue({{sup::dto::SignedInteger32Type, 42}, 43}, "array_name1");
  auto array_value2 =
      sup::dto::ArrayValue({{sup::dto::SignedInteger32Type, 44}, 45, 46}, "array_name2");
  const sup::dto::AnyValue expected_struct_value = {
      {{"field1", array_value1}, {"field2", array_value2}}, "struct_name"};

  AnyValueStructItem item;
  item.SetAnyTypeName("struct_name");

  auto array0 = item.InsertItem<AnyValueArrayItem>(mvvm::TagIndex::Append());
  array0->SetAnyTypeName("array_name1");
  array0->SetDisplayName("field1");

  auto element0 = array0->InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  element0->SetAnyTypeName(sup::dto::kInt32TypeName);
  element0->SetData(42);

  auto element1 = array0->InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  element1->SetAnyTypeName(sup::dto::kInt32TypeName);
  element1->SetData(43);

  auto array1 = item.InsertItem<AnyValueArrayItem>(mvvm::TagIndex::Append());
  array1->SetAnyTypeName("array_name2");
  array1->SetDisplayName("field2");

  auto element2 = array1->InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  element2->SetAnyTypeName(sup::dto::kInt32TypeName);
  element2->SetData(44);

  auto element3 = array1->InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  element3->SetAnyTypeName(sup::dto::kInt32TypeName);
  element3->SetData(45);

  auto element4 = array1->InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  element4->SetAnyTypeName(sup::dto::kInt32TypeName);
  element4->SetData(46);

  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected_struct_value);
}

//! Building an array with two structure elements.

TEST_F(DomainAnyValueBuilderTest, ArrayWithTwoStructureElements)
{
  sup::dto::AnyValue struct_value1 = {{{"first", {sup::dto::SignedInteger16Type, -43}},
                                       {"second", {sup::dto::UnsignedInteger16Type, 44}}},
                                      "struct_name"};
  const sup::dto::AnyValue struct_value2 = {{{"first", {sup::dto::SignedInteger16Type, 42}},
                                             {"second", {sup::dto::UnsignedInteger16Type, 43}}},
                                            "struct_name"};

  auto expected_array_value = sup::dto::ArrayValue({{struct_value1}, struct_value2}, "array_name");

  AnyValueArrayItem item;
  item.SetAnyTypeName("array_name");

  auto struct0 = item.InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  struct0->SetAnyTypeName("struct_name");
  const mvvm::int16 num1(-43);
  const mvvm::uint16 num2(44);
  struct0->AddScalarField("first", sup::dto::kInt16TypeName, num1);
  struct0->AddScalarField("second", sup::dto::kUInt16TypeName, num2);

  auto struct1 = item.InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  struct1->SetAnyTypeName("struct_name");
  const mvvm::int16 num3(42);
  const mvvm::uint16 num4(43);
  struct1->AddScalarField("first", sup::dto::kInt16TypeName, num3);
  struct1->AddScalarField("second", sup::dto::kUInt16TypeName, num4);

  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected_array_value);
}

//! Building a structure with array as a field with two structures.

TEST_F(DomainAnyValueBuilderTest, StructureWithArrayWithStructure)
{
  const sup::dto::AnyValue struct_value1 = {{{"first", {sup::dto::SignedInteger64Type, -43}},
                                             {"second", {sup::dto::UnsignedInteger64Type, 44}}},
                                            "struct_name"};
  const sup::dto::AnyValue struct_value2 = {{{"first", {sup::dto::SignedInteger64Type, 42}},
                                             {"second", {sup::dto::UnsignedInteger64Type, 43}}},
                                            "struct_name"};

  auto array_value = sup::dto::ArrayValue({{struct_value1}, struct_value2}, "array_name");

  const sup::dto::AnyValue expected_struct_value = {{{"field", array_value}}, "struct_name2"};

  AnyValueStructItem item;
  item.SetAnyTypeName("struct_name2");

  auto array = item.InsertItem<AnyValueArrayItem>(mvvm::TagIndex::Append());
  array->SetAnyTypeName("array_name");
  array->SetDisplayName("field");

  auto struct1 = array->InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  struct1->SetAnyTypeName("struct_name");
  const mvvm::int64 num1(-43);
  const mvvm::uint64 num2(44);
  struct1->AddScalarField("first", sup::dto::kInt64TypeName, num1);
  struct1->AddScalarField("second", sup::dto::kUInt64TypeName, num2);

  auto struct2 = array->InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  struct2->SetAnyTypeName("struct_name");
  const mvvm::int64 num3(42);
  const mvvm::uint64 num4(43);
  struct2->AddScalarField("first", sup::dto::kInt64TypeName, num3);
  struct2->AddScalarField("second", sup::dto::kUInt64TypeName, num4);

  auto any_value = CreateAnyValue(item);
  EXPECT_EQ(any_value, expected_struct_value);
}

}  // namespace sup::gui::test
