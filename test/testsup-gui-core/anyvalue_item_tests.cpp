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

#include <sup/gui/model/anyvalue_item_constants.h>
#include <sup/gui/model/scalartype_property_item.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/test/test_helper.h>

#include <sup/dto/anytype.h>

#include <gtest/gtest.h>

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
  }

  {  // AnyValueScalarItem
    const AnyValueScalarItem item;
    EXPECT_TRUE(item.IsScalar());
    EXPECT_FALSE(item.IsStruct());
    EXPECT_FALSE(item.IsArray());
    EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kBooleanTypeName);
    EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
    EXPECT_TRUE(mvvm::utils::HasTag(item, constants::kAnyValueTypeTag));
    EXPECT_TRUE(item.GetChildren().empty());
    EXPECT_EQ(item.GetDisplayName(), constants::kScalarTypeName);
    EXPECT_TRUE(item.HasFlag(mvvm::Appearance::kProperty));
    EXPECT_EQ(item.GetChildrenCount(), 0);
  }

  {  // AnyValueStructItem
    const AnyValueStructItem item;
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
  }

  {  // AnyValueArrayItem
    const AnyValueArrayItem item;
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
  }
}

TEST_F(AnyValueItemTest, SetAnyTypeName)
{
  {  // AnyValueScalarItem
    AnyValueScalarItem item;

    item.SetAnyTypeName(sup::dto::kInt8TypeName);

    EXPECT_TRUE(item.IsScalar());
    EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);
    EXPECT_EQ(item.Data<mvvm::int8>(), 0);
    EXPECT_EQ(item.GetToolTip(), sup::dto::kInt8TypeName);
    EXPECT_TRUE(mvvm::utils::IsValid(item.Data()));
    EXPECT_TRUE(item.HasData(mvvm::DataRole::kData));
    EXPECT_TRUE(mvvm::utils::HasTag(item, constants::kAnyValueTypeTag));

    // change scalar type
    item.SetAnyTypeName(sup::dto::kBooleanTypeName);
    EXPECT_TRUE(item.IsScalar());
    EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kBooleanTypeName);
    EXPECT_EQ(item.Data<bool>(), false);
    EXPECT_EQ(item.GetToolTip(), sup::dto::kBooleanTypeName);
    EXPECT_TRUE(mvvm::utils::IsValid(item.Data()));
    EXPECT_TRUE(item.HasData(mvvm::DataRole::kData));
    EXPECT_TRUE(mvvm::utils::HasTag(item, constants::kAnyValueTypeTag));
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
  auto scalar = item.AddScalarField("signed", sup::dto::kInt32TypeName, 42);

  EXPECT_EQ(scalar->GetDisplayName(), "signed");
  EXPECT_EQ(scalar->GetAnyTypeName(), sup::dto::kInt32TypeName);
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

TEST_F(AnyValueItemTest, SetNewScalarTypeNameViaPropertyItem)
{
  AnyValueScalarItem item;

  item.SetAnyTypeName(sup::dto::kInt8TypeName);

  EXPECT_TRUE(item.IsScalar());
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kInt8TypeName);
  EXPECT_EQ(item.Data<mvvm::int8>(), 0);
  EXPECT_EQ(item.GetToolTip(), sup::dto::kInt8TypeName);
  EXPECT_TRUE(mvvm::utils::IsValid(item.Data()));
  EXPECT_TRUE(item.HasData(mvvm::DataRole::kData));
  EXPECT_TRUE(mvvm::utils::HasTag(item, constants::kAnyValueTypeTag));

  // change scalar type via property item
  auto property_item =  item.GetItem<ScalarTypePropertyItem>(constants::kAnyValueTypeTag);
  property_item->SetScalarTypeName(sup::dto::kBooleanTypeName);
  EXPECT_TRUE(item.IsScalar());
  EXPECT_EQ(item.GetAnyTypeName(), sup::dto::kBooleanTypeName);
  EXPECT_EQ(item.Data<bool>(), false);
  EXPECT_EQ(item.GetToolTip(), sup::dto::kBooleanTypeName);
  EXPECT_TRUE(mvvm::utils::IsValid(item.Data()));
  EXPECT_TRUE(item.HasData(mvvm::DataRole::kData));
  EXPECT_TRUE(mvvm::utils::HasTag(item, constants::kAnyValueTypeTag));
}

}  // namespace sup::gui::test
