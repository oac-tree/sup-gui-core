/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sup/gui/model/anyvalue_item.h"

#include <sup/dto/anytype.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

namespace
{
const int kAnyTypeNameRole = 10;  // defined in anyvalue_item.cpp
}

using namespace sup::gui;

class AnyValueItemTest : public ::testing::Test
{
public:
  //! Returns true if clone method is implemented.
  template <typename T>
  bool IsCloneImplemented()
  {
    // We expect that the specified object can be created, cloned, and the result of clone can
    // be casted to the object type itself.
    T item;
    auto clone = item.Clone(/*make_unique_id*/ false);
    return testutils::CanCast<T>(clone.get());
  }
};

TEST_F(AnyValueItemTest, InitialState)
{
  {  // AnyValueItem
    const AnyValueItem item("test");
    EXPECT_FALSE(item.IsScalar());
    EXPECT_FALSE(item.IsStruct());
    EXPECT_FALSE(item.IsArray());
    EXPECT_TRUE(item.GetAnyTypeName().empty());
    EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
    EXPECT_FALSE(item.HasData(kAnyTypeNameRole));
    EXPECT_TRUE(item.HasFlag(mvvm::Appearance::kProperty));
    EXPECT_EQ(item.GetChildrenCount(), 0);
  }

  {  // AnyValueScalarItem
    const AnyValueScalarItem item;
    EXPECT_TRUE(item.IsScalar());
    EXPECT_FALSE(item.IsStruct());
    EXPECT_FALSE(item.IsArray());
    EXPECT_TRUE(item.GetAnyTypeName().empty());
    EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
    EXPECT_FALSE(item.HasData(kAnyTypeNameRole));
    EXPECT_TRUE(item.GetChildren().empty());
    EXPECT_EQ(item.GetDisplayName(), kScalarTypeName);
    EXPECT_TRUE(item.HasFlag(mvvm::Appearance::kProperty));
    EXPECT_EQ(item.GetChildrenCount(), 0);
  }

  {  // AnyValueStructItem
    const AnyValueStructItem item;
    EXPECT_FALSE(item.IsScalar());
    EXPECT_TRUE(item.IsStruct());
    EXPECT_FALSE(item.IsArray());
    EXPECT_EQ(item.GetAnyTypeName(), std::string());
    EXPECT_EQ(item.GetDisplayName(), kStructTypeName);
    EXPECT_FALSE(mvvm::utils::IsValid(item.Data()));
    EXPECT_FALSE(item.HasData(mvvm::DataRole::kData));
    EXPECT_TRUE(item.HasData(kAnyTypeNameRole)); // default name "" is there to allow editing
    EXPECT_TRUE(item.GetChildren().empty());
    EXPECT_EQ(item.GetDisplayName(), kStructTypeName);
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
    EXPECT_TRUE(item.HasData(kAnyTypeNameRole));  // default name "" is there to allow editing
    EXPECT_TRUE(item.GetChildren().empty());
    EXPECT_EQ(item.GetDisplayName(), kArrayTypeName);
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
    EXPECT_TRUE(mvvm::utils::IsValid(item.Data()));
    EXPECT_TRUE(item.HasData(mvvm::DataRole::kData));
    EXPECT_TRUE(item.HasData(kAnyTypeNameRole));
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
  EXPECT_TRUE(IsCloneImplemented<AnyValueEmptyItem>());
  EXPECT_TRUE(IsCloneImplemented<AnyValueScalarItem>());
  EXPECT_TRUE(IsCloneImplemented<AnyValueStructItem>());
  EXPECT_TRUE(IsCloneImplemented<AnyValueArrayItem>());
}
