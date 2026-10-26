/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/serialization/xml_document.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

namespace sup::gui::test
{

class AnyValueItemTest : public ::testing::Test
{
};

TEST_F(AnyValueItemTest, InitialState)
{
  {  // AnyValueItem
    const AnyValueItem item("test");
    EXPECT_FALSE(item.IsScalar());
    EXPECT_FALSE(item.IsStruct());
    EXPECT_FALSE(item.IsArray());
    EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
    EXPECT_FALSE(mvvm::utils::HasTag(item, constants::kAnyValueTypeTag));
    EXPECT_TRUE(item.HasFlag(mvvm::Appearance::kProperty));
    EXPECT_EQ(item.GetChildrenCount(), 0);
  }

  {  // AnyValueEmptyItem
    const AnyValueEmptyItem item;

    EXPECT_EQ(mvvm::GetTypeName<AnyValueEmptyItem>(), "AnyValueEmpty");
    EXPECT_EQ(mvvm::GetTypeName<AnyValueEmptyItem>(), item.GetType());

    EXPECT_FALSE(item.IsScalar());
    EXPECT_FALSE(item.IsStruct());
    EXPECT_FALSE(item.IsArray());
    EXPECT_EQ(item.GetAnyTypeName(), constants::kEmptyTypeName);
    EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
    EXPECT_TRUE(mvvm::utils::HasTag(item, constants::kAnyValueTypeTag));
    EXPECT_TRUE(item.GetChildren().empty());
    EXPECT_EQ(item.GetDisplayName(), constants::kEmptyTypeName);
    EXPECT_TRUE(item.HasFlag(mvvm::Appearance::kProperty));
    EXPECT_EQ(item.GetChildrenCount(), 0);
    EXPECT_FALSE(item.IsEditable());
  }

  {  // AnyValueScalarItem
    const AnyValueScalarItem item;

    EXPECT_EQ(mvvm::GetTypeName<AnyValueScalarItem>(), "AnyValueScalar");
    EXPECT_EQ(mvvm::GetTypeName<AnyValueScalarItem>(), item.GetType());

    EXPECT_TRUE(item.IsScalar());
    EXPECT_FALSE(item.IsStruct());
    EXPECT_FALSE(item.IsArray());
    EXPECT_EQ(item.GetAnyTypeName(), std::string());
    EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
    EXPECT_FALSE(mvvm::utils::HasTag(item, constants::kAnyValueTypeTag));
    EXPECT_TRUE(item.GetChildren().empty());
    EXPECT_EQ(item.GetDisplayName(), constants::kScalarTypeName);
    EXPECT_TRUE(item.HasFlag(mvvm::Appearance::kProperty));
    EXPECT_EQ(item.GetChildrenCount(), 0);
  }

  {  // AnyValueStructItem
    const AnyValueStructItem item;

    EXPECT_EQ(mvvm::GetTypeName<AnyValueStructItem>(), "AnyValueStruct");
    EXPECT_EQ(mvvm::GetTypeName<AnyValueStructItem>(), item.GetType());

    EXPECT_FALSE(item.IsScalar());
    EXPECT_TRUE(item.IsStruct());
    EXPECT_FALSE(item.IsArray());
    EXPECT_EQ(item.GetAnyTypeName(), std::string());
    EXPECT_EQ(item.GetDisplayName(), constants::kStructTypeName);
    EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
    EXPECT_TRUE(mvvm::utils::HasTag(item, constants::kAnyValueTypeTag));
    EXPECT_TRUE(item.GetChildren().empty());
    EXPECT_EQ(item.GetDisplayName(), constants::kStructTypeName);
    EXPECT_TRUE(item.HasFlag(mvvm::Appearance::kProperty));
    EXPECT_EQ(item.GetChildrenCount(), 0);
    EXPECT_FALSE(item.IsEditable());
  }

  {  // AnyValueArrayItem
    const AnyValueArrayItem item;

    EXPECT_EQ(mvvm::GetTypeName<AnyValueArrayItem>(), "AnyValueArray");
    EXPECT_EQ(mvvm::GetTypeName<AnyValueArrayItem>(), item.GetType());

    EXPECT_FALSE(item.IsScalar());
    EXPECT_FALSE(item.IsStruct());
    EXPECT_TRUE(item.IsArray());
    EXPECT_EQ(item.GetAnyTypeName(), std::string());
    EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
    EXPECT_TRUE(mvvm::utils::HasTag(item, constants::kAnyValueTypeTag));
    EXPECT_TRUE(item.GetChildren().empty());
    EXPECT_EQ(item.GetDisplayName(), constants::kArrayTypeName);
    EXPECT_TRUE(item.HasFlag(mvvm::Appearance::kProperty));
    EXPECT_EQ(item.GetChildrenCount(), 0);
    EXPECT_FALSE(item.IsEditable());
  }
}

TEST_F(AnyValueItemTest, SetAnyTypeName)
{
  {  // AnyValueScalarItem (set boolean)
    AnyValueScalarItem item;

    item.SetAnyTypeName(mvvm::constants::kBooleanTypeName);

    EXPECT_TRUE(item.IsScalar());
    EXPECT_EQ(item.GetAnyTypeName(), mvvm::constants::kBooleanTypeName);
    EXPECT_EQ(item.Data<mvvm::boolean>(), false);
    EXPECT_EQ(item.GetToolTip(), mvvm::constants::kBooleanTypeName);
    EXPECT_TRUE(mvvm::utils::IsValid(item.Data()));
    EXPECT_TRUE(item.HasData(mvvm::DataRole::kData));
    EXPECT_FALSE(mvvm::utils::HasTag(item, constants::kAnyValueTypeTag));
  }

  {  // AnyValueScalarItem (change type)
    AnyValueScalarItem item;

    item.SetAnyTypeName(mvvm::constants::kInt8TypeName);

    EXPECT_TRUE(item.IsScalar());
    EXPECT_EQ(item.GetAnyTypeName(), mvvm::constants::kInt8TypeName);
    EXPECT_EQ(item.Data<mvvm::int8>(), 0);
    EXPECT_EQ(item.GetToolTip(), mvvm::constants::kInt8TypeName);
    EXPECT_TRUE(mvvm::utils::IsValid(item.Data()));
    EXPECT_TRUE(item.HasData(mvvm::DataRole::kData));
    EXPECT_FALSE(mvvm::utils::HasTag(item, constants::kAnyValueTypeTag));

    // change scalar type
    item.SetAnyTypeName(mvvm::constants::kBooleanTypeName);
    EXPECT_TRUE(item.IsScalar());
    EXPECT_EQ(item.GetAnyTypeName(), mvvm::constants::kBooleanTypeName);
    EXPECT_EQ(item.Data<bool>(), false);
    EXPECT_EQ(item.GetToolTip(), mvvm::constants::kBooleanTypeName);
    EXPECT_TRUE(mvvm::utils::IsValid(item.Data()));
    EXPECT_TRUE(item.HasData(mvvm::DataRole::kData));
    EXPECT_FALSE(mvvm::utils::HasTag(item, constants::kAnyValueTypeTag));
  }

  {  // AnyValueStructItem
    AnyValueStructItem item;

    item.SetAnyTypeName("abc");

    EXPECT_FALSE(item.IsScalar());
    EXPECT_TRUE(item.IsStruct());
    EXPECT_EQ(item.GetAnyTypeName(), std::string("abc"));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
  }
}

TEST_F(AnyValueItemTest, AddScalarField)
{
  AnyValueStructItem item;
  auto scalar = item.AddScalarField("signed", 42);

  EXPECT_EQ(scalar->GetDisplayName(), "signed");
  EXPECT_EQ(scalar->GetAnyTypeName(), mvvm::constants::kInt32TypeName);
  EXPECT_EQ(scalar->Data<int>(), 42);
  EXPECT_EQ(item.GetChildren(), std::vector<AnyValueItem*>({scalar}));
  EXPECT_EQ(item.GetChildrenCount(), 1);
}

TEST_F(AnyValueItemTest, Clone)
{
  EXPECT_TRUE(mvvm::test::IsCloneImplemented<AnyValueEmptyItem>());
  EXPECT_TRUE(mvvm::test::IsCloneImplemented<AnyValueScalarItem>());
  EXPECT_TRUE(mvvm::test::IsCloneImplemented<AnyValueStructItem>());
  EXPECT_TRUE(mvvm::test::IsCloneImplemented<AnyValueArrayItem>());
}

//! Scalar type is the type of the data for all scalar types.
TEST_F(AnyValueItemTest, ScalarTypeFromData)
{
  for (const auto& type_name : GetScalarTypeNames())
  {
    AnyValueScalarItem item;
    item.SetAnyTypeName(type_name);

    EXPECT_EQ(item.GetAnyTypeName(), type_name);
    EXPECT_EQ(mvvm::utils::TypeName(item.Data()), type_name);
    EXPECT_EQ(item.GetToolTip(), type_name);
    EXPECT_EQ(item.GetTotalItemCount(), 0);
  }
}

//! Data on a scalar without type defines the type.
TEST_F(AnyValueItemTest, ScalarTypeOfUntypedScalar)
{
  AnyValueScalarItem item;
  EXPECT_TRUE(item.SetData(mvvm::int16{3}));
  EXPECT_EQ(item.GetAnyTypeName(), mvvm::constants::kInt16TypeName);
}

//! Data of another type doesn't change the type of a typed scalar.
TEST_F(AnyValueItemTest, ScalarTypeOnIncompatibleData)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);

  EXPECT_THROW((void)item.SetData(std::string("abc")), std::exception);
  EXPECT_EQ(item.GetAnyTypeName(), mvvm::constants::kInt32TypeName);
}

//! Setting the current type, or an empty type, keeps the value.
TEST_F(AnyValueItemTest, SetSameScalarTypeName)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);
  (void)item.SetData(mvvm::int32{42});

  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);
  EXPECT_EQ(item.Data<mvvm::int32>(), 42);

  item.SetAnyTypeName(std::string());
  EXPECT_EQ(item.GetAnyTypeName(), mvvm::constants::kInt32TypeName);
  EXPECT_EQ(item.Data<mvvm::int32>(), 42);
}

//! Unknown type name throws and leaves the scalar unchanged.
TEST_F(AnyValueItemTest, SetUnknownScalarTypeName)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);
  (void)item.SetData(mvvm::int32{42});

  EXPECT_THROW(item.SetAnyTypeName("abc"), RuntimeException);
  EXPECT_EQ(item.GetAnyTypeName(), mvvm::constants::kInt32TypeName);
  EXPECT_EQ(item.Data<mvvm::int32>(), 42);
}

//! Clone keeps the scalar type and value, and its type can be changed.
TEST_F(AnyValueItemTest, ScalarClone)
{
  AnyValueScalarItem orig;
  orig.SetAnyTypeName(mvvm::constants::kInt8TypeName);
  (void)orig.SetData(mvvm::int8{42});

  auto clone = mvvm::utils::CloneItem(orig);
  EXPECT_EQ(clone->GetAnyTypeName(), mvvm::constants::kInt8TypeName);
  EXPECT_EQ(clone->Data<mvvm::int8>(), mvvm::int8{42});

  clone->SetAnyTypeName(mvvm::constants::kBooleanTypeName);
  EXPECT_EQ(clone->GetAnyTypeName(), mvvm::constants::kBooleanTypeName);
  EXPECT_EQ(clone->Data<bool>(), false);
  EXPECT_EQ(orig.GetAnyTypeName(), mvvm::constants::kInt8TypeName);
}

//! Tests for AnyValueScalarItem serialization.
class AnyValueScalarItemXmlTest : public FolderTest
{
public:
  AnyValueScalarItemXmlTest() : FolderTest("AnyValueScalarItemXmlTest") {}
};

//! Scalar types with the same Qt representation survive the save and load.
TEST_F(AnyValueScalarItemXmlTest, XmlDocumentSaveLoad)
{
  const auto file_path = GetFilePath("XmlDocumentSaveLoad.xml");

  mvvm::ApplicationModel model;
  auto struct_item = model.InsertItem<AnyValueStructItem>();
  (void)struct_item->AddScalarField("int8", mvvm::int8{-1});
  (void)struct_item->AddScalarField("char8", mvvm::char8{'a'});
  (void)struct_item->AddScalarField("uint64", mvvm::uint64{42});
  (void)struct_item->AddScalarField("float32", mvvm::float32{1.5});

  mvvm::XmlDocument document({&model});
  document.Save(file_path);
  model.Clear();
  document.Load(file_path);

  auto restored = mvvm::utils::GetTopItem<AnyValueStructItem>(model);
  ASSERT_NE(restored, nullptr);
  auto children = restored->GetChildren();
  ASSERT_EQ(children.size(), 4U);
  EXPECT_EQ(children.at(0)->GetAnyTypeName(), mvvm::constants::kInt8TypeName);
  EXPECT_EQ(children.at(0)->Data<mvvm::int8>(), mvvm::int8{-1});
  EXPECT_EQ(children.at(1)->GetAnyTypeName(), mvvm::constants::kChar8TypeName);
  EXPECT_EQ(children.at(1)->Data<mvvm::char8>(), mvvm::char8{'a'});
  EXPECT_EQ(children.at(2)->GetAnyTypeName(), mvvm::constants::kUInt64TypeName);
  EXPECT_EQ(children.at(2)->Data<mvvm::uint64>(), mvvm::uint64{42});
  EXPECT_EQ(children.at(3)->GetAnyTypeName(), mvvm::constants::kFloat32TypeName);
  EXPECT_EQ(children.at(3)->Data<mvvm::float32>(), mvvm::float32{1.5});
}

}  // namespace sup::gui::test
