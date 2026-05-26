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

#include "sup/gui/model/anyvalue_item_utils.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/test/mock_model_listener.h>

#include <gtest/gtest.h>

#include <stdexcept>

namespace sup::gui::test
{

/**
 * @brief Tests for helper methods from anyvalie_item_utils.h
 */
class AnyValueItemUtilsTest : public ::testing::Test
{
public:
  using mock_listener_t = ::testing::StrictMock<mvvm::test::MockModelListener>;
};

TEST_F(AnyValueItemUtilsTest, GetAnyValueItemTypeFromTypeName)
{
  EXPECT_TRUE(GetAnyValueItemTypeFromTypeName(std::string()).empty());
  EXPECT_TRUE(GetAnyValueItemTypeFromTypeName(std::string("not-anyvalue-item-type")).empty());

  EXPECT_EQ(GetAnyValueItemTypeFromTypeName(constants::kEmptyTypeName),
            mvvm::GetTypeName<AnyValueEmptyItem>());
  EXPECT_EQ(GetAnyValueItemTypeFromTypeName(constants::kScalarTypeName),
            mvvm::GetTypeName<AnyValueScalarItem>());
  EXPECT_EQ(GetAnyValueItemTypeFromTypeName(constants::kArrayTypeName),
            mvvm::GetTypeName<AnyValueArrayItem>());
  EXPECT_EQ(GetAnyValueItemTypeFromTypeName(constants::kStructTypeName),
            mvvm::GetTypeName<AnyValueStructItem>());

  EXPECT_EQ(GetAnyValueItemTypeFromTypeName(mvvm::constants::kInt32TypeName),
            mvvm::GetTypeName<AnyValueScalarItem>());
  EXPECT_EQ(GetAnyValueItemTypeFromTypeName(mvvm::constants::kBooleanTypeName),
            mvvm::GetTypeName<AnyValueScalarItem>());
}

TEST_F(AnyValueItemUtilsTest, CreateAnyValueItemFromTypeName)
{
  // basic items
  EXPECT_EQ(CreateAnyValueItemFromTypeName(constants::kEmptyTypeName)->GetType(),
            mvvm::GetTypeName<AnyValueEmptyItem>());
  EXPECT_EQ(CreateAnyValueItemFromTypeName(constants::kScalarTypeName)->GetType(),
            mvvm::GetTypeName<AnyValueScalarItem>());
  EXPECT_EQ(CreateAnyValueItemFromTypeName(constants::kArrayTypeName)->GetType(),
            mvvm::GetTypeName<AnyValueArrayItem>());
  EXPECT_EQ(CreateAnyValueItemFromTypeName(constants::kStructTypeName)->GetType(),
            mvvm::GetTypeName<AnyValueStructItem>());

  // scalar items
  auto scalar = CreateAnyValueItemFromTypeName("int32");
  EXPECT_EQ(scalar->GetType(), mvvm::GetTypeName<AnyValueScalarItem>());
  EXPECT_EQ(scalar->GetAnyTypeName(), "int32");

  EXPECT_THROW(CreateAnyValueItemFromTypeName("int42"), mvvm::RuntimeException);
}

TEST_F(AnyValueItemUtilsTest, UpdateAnyValueItemScalarData)
{
  {  // empty items
    const AnyValueEmptyItem source;
    AnyValueEmptyItem target;
    EXPECT_THROW(UpdateAnyValueItemScalarData(source, target), std::logic_error);
  }

  {  // two scalars, same type
    AnyValueScalarItem source;
    source.SetAnyTypeName(mvvm::constants::kInt32TypeName);
    source.SetData(42);

    AnyValueScalarItem target;
    target.SetAnyTypeName(mvvm::constants::kInt8TypeName);

    EXPECT_THROW(UpdateAnyValueItemScalarData(source, target), std::logic_error);
  }

  {  // two scalars, same type
    AnyValueScalarItem source;
    source.SetAnyTypeName(mvvm::constants::kInt32TypeName);
    source.SetData(42);

    AnyValueScalarItem target;
    target.SetAnyTypeName(mvvm::constants::kInt32TypeName);
    target.SetData(0);
    EXPECT_EQ(target.Data<int>(), 0);

    EXPECT_NO_THROW(UpdateAnyValueItemScalarData(source, target));
    EXPECT_EQ(target.Data<int>(), 42);
  }
}

TEST_F(AnyValueItemUtilsTest, UpdateAnyValueItemDataFromEmptyItem)
{
  // Updating one empty item from another. Nothing wrong is expected.
  const AnyValueEmptyItem source;
  AnyValueEmptyItem target;
  EXPECT_NO_THROW(UpdateAnyValueItemData(source, target));
}

TEST_F(AnyValueItemUtilsTest, UpdateAnyValueScalarItemDataFromAnotherScalar)
{
  // the value of the target should change
  AnyValueScalarItem source;
  source.SetAnyTypeName(mvvm::constants::kInt32TypeName);
  source.SetData(42);

  AnyValueScalarItem target;
  target.SetAnyTypeName(mvvm::constants::kInt32TypeName);
  target.SetData(0);
  EXPECT_EQ(target.Data<int>(), 0);

  EXPECT_NO_THROW(UpdateAnyValueItemData(source, target));
  EXPECT_EQ(target.Data<int>(), 42);
}

TEST_F(AnyValueItemUtilsTest, SignalingWhileUpdatingAnyValueItemDataFromScalar)
{
  mvvm::ApplicationModel model;
  auto source = model.InsertItem<AnyValueScalarItem>();
  source->SetAnyTypeName(mvvm::constants::kInt32TypeName);
  source->SetData(42);

  auto target = model.InsertItem<AnyValueScalarItem>();
  target->SetAnyTypeName(mvvm::constants::kInt32TypeName);
  target->SetData(0);
  EXPECT_EQ(target->Data<int>(), 0);

  mock_listener_t listener(&model);

  EXPECT_CALL(listener, OnDataChanged(::testing::_)).Times(1);

  EXPECT_NO_THROW(UpdateAnyValueItemData(*source, *target));
  EXPECT_EQ(target->Data<int>(), 42);
}

TEST_F(AnyValueItemUtilsTest, SignalingWhileUpdatingAnyValueItemDataFromSameScalar)
{
  mvvm::ApplicationModel model;
  auto source = model.InsertItem<AnyValueScalarItem>();
  source->SetAnyTypeName(mvvm::constants::kInt32TypeName);
  source->SetData(42);

  auto target = model.InsertItem<AnyValueScalarItem>();
  target->SetAnyTypeName(mvvm::constants::kInt32TypeName);
  target->SetData(42);

  mock_listener_t listener(&model);

  EXPECT_CALL(listener, OnDataChanged(::testing::_)).Times(0);

  EXPECT_NO_THROW(UpdateAnyValueItemData(*source, *target));
  EXPECT_EQ(target->Data<int>(), 42);
}

TEST_F(AnyValueItemUtilsTest, UpdateAnyValueItemDataFromDifferentStructs)
{
  // Attempt to update structure from the source with different layout.
  AnyValueStructItem source;
  source.AddScalarField("signed", 42);

  AnyValueStructItem target;

  EXPECT_THROW(UpdateAnyValueItemData(source, target), RuntimeException);
}

TEST_F(AnyValueItemUtilsTest, UpdateAnyValueItemDataFromStructWithSingleField)
{
  AnyValueStructItem source;
  source.AddScalarField("signed", 42);

  AnyValueStructItem target;
  auto scalar1 = target.AddScalarField("signed", 43);

  EXPECT_NO_THROW(UpdateAnyValueItemData(source, target));
  EXPECT_EQ(scalar1->Data<int>(), 42);
}

TEST_F(AnyValueItemUtilsTest, UpdateAnyValueItemDataFromStructWithNestedField)
{
  // same as above with nested fields
  AnyValueStructItem source;
  auto source_inner_struct = source.InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  source_inner_struct->SetDisplayName("struct0");
  source_inner_struct->AddScalarField("value", 42);
  source_inner_struct->AddScalarField("flag", true);

  AnyValueStructItem target;
  auto targets_inner_struct = target.InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  targets_inner_struct->SetDisplayName("struct0");
  auto target_scalar0 = targets_inner_struct->AddScalarField("value", 0);
  auto target_scalar1 = targets_inner_struct->AddScalarField("flag", false);

  EXPECT_NO_THROW(UpdateAnyValueItemData(source, target));
  EXPECT_EQ(target_scalar0->Data<int>(), 42);
  EXPECT_EQ(target_scalar1->Data<bool>(), true);
}

TEST_F(AnyValueItemUtilsTest, IsSuitableScalarType)
{
  {  // empty array
    const AnyValueArrayItem item;
    EXPECT_TRUE(IsSuitableScalarType(item, mvvm::constants::kInt32TypeName));
  }

  {  // array with matching type
    AnyValueArrayItem item;
    item.InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append())
        ->SetAnyTypeName(mvvm::constants::kInt32TypeName);
    EXPECT_TRUE(IsSuitableScalarType(item, mvvm::constants::kInt32TypeName));
    EXPECT_FALSE(IsSuitableScalarType(item, mvvm::constants::kInt16TypeName));
  }

  {  // array with wrong type
    AnyValueArrayItem item;
    item.InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append())
        ->SetAnyTypeName(mvvm::constants::kInt32TypeName);
    item.InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append())
        ->SetAnyTypeName(mvvm::constants::kInt32TypeName);
    EXPECT_FALSE(IsSuitableScalarType(item, mvvm::constants::kInt16TypeName));
  }
}

TEST_F(AnyValueItemUtilsTest, GetAnyValueItemTypes)
{
  const auto types = GetAnyValueItemTypes();

  EXPECT_EQ(types.size(), 4);

  EXPECT_NE(std::find(types.begin(), types.end(), mvvm::GetTypeName<AnyValueEmptyItem>()),
            types.end());
  EXPECT_NE(std::find(types.begin(), types.end(), mvvm::GetTypeName<AnyValueScalarItem>()),
            types.end());
  EXPECT_NE(std::find(types.begin(), types.end(), mvvm::GetTypeName<AnyValueStructItem>()),
            types.end());
  EXPECT_NE(std::find(types.begin(), types.end(), mvvm::GetTypeName<AnyValueArrayItem>()),
            types.end());
}

TEST_F(AnyValueItemUtilsTest, CreateAnyValueTag)
{
  auto tag = CreateAnyValueTag("tag");

  EXPECT_TRUE(tag.IsValidType(mvvm::GetTypeName<AnyValueEmptyItem>()));
  EXPECT_TRUE(tag.IsValidType(mvvm::GetTypeName<AnyValueScalarItem>()));
  EXPECT_TRUE(tag.IsValidType(mvvm::GetTypeName<AnyValueStructItem>()));
  EXPECT_TRUE(tag.IsValidType(mvvm::GetTypeName<AnyValueArrayItem>()));

  EXPECT_FALSE(tag.IsValidType(mvvm::GetTypeName<mvvm::SessionItem>()));
}

}  // namespace sup::gui::test
