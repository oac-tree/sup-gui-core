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

#include "sup/gui/model/anyvalue_item_utils.h"

#include <sup/gui/core/exceptions.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>

#include <sup/dto/anytype.h>

#include <gtest/gtest.h>
#include <testutils/mock_model_listener.h>

#include <stdexcept>

using namespace sup::gui;
using ::testing::_;

class AnyValueItemUtilsTest : public ::testing::Test
{
public:
  using mock_listener_t = ::testing::StrictMock<testutils::MockModelListener>;
};

//! Testing UpdateAnyValueItemScalarData method.

TEST_F(AnyValueItemUtilsTest, UpdateAnyValueItemScalarData)
{
  {  // empty items
    AnyValueEmptyItem source;
    AnyValueEmptyItem target;
    EXPECT_THROW(UpdateAnyValueItemScalarData(source, target), std::logic_error);
  }

  {  // two scalars, same type
    AnyValueScalarItem source;
    source.SetAnyTypeName(sup::dto::kInt32TypeName);
    source.SetData(42);

    AnyValueScalarItem target;
    target.SetAnyTypeName(sup::dto::kInt8TypeName);

    EXPECT_THROW(UpdateAnyValueItemScalarData(source, target), std::logic_error);
  }

  {  // two scalars, same type
    AnyValueScalarItem source;
    source.SetAnyTypeName(sup::dto::kInt32TypeName);
    source.SetData(42);

    AnyValueScalarItem target;
    target.SetAnyTypeName(sup::dto::kInt32TypeName);
    target.SetData(0);
    EXPECT_EQ(target.Data<int>(), 0);

    EXPECT_NO_THROW(UpdateAnyValueItemScalarData(source, target));
    EXPECT_EQ(target.Data<int>(), 42);
  }
}

//! Testing UpdateAnyValueItemData method.
//! Updating one empty item from another. Nothing wrong is expected.

TEST_F(AnyValueItemUtilsTest, UpdateAnyValueItemDataFromEmptyItem)
{
  AnyValueEmptyItem source;
  AnyValueEmptyItem target;
  EXPECT_NO_THROW(UpdateAnyValueItemData(source, target));
}

//! Testing UpdateAnyValueItemData method.
//! Updating one scalar item from another scalar item. The value of the target should change.

TEST_F(AnyValueItemUtilsTest, UpdateAnyValueItemDataFromScalar)
{
  AnyValueScalarItem source;
  source.SetAnyTypeName(sup::dto::kInt32TypeName);
  source.SetData(42);

  AnyValueScalarItem target;
  target.SetAnyTypeName(sup::dto::kInt32TypeName);
  target.SetData(0);
  EXPECT_EQ(target.Data<int>(), 0);

  EXPECT_NO_THROW(UpdateAnyValueItemData(source, target));
  EXPECT_EQ(target.Data<int>(), 42);
}

//! Testing signaling while updating the data with UpdateAnyValueItemData method.

TEST_F(AnyValueItemUtilsTest, SignalingWhileUpdatingAnyValueItemDataFromScalar)
{
  mvvm::ApplicationModel model;
  auto source = model.InsertItem<AnyValueScalarItem>();
  source->SetAnyTypeName(sup::dto::kInt32TypeName);
  source->SetData(42);

  auto target = model.InsertItem<AnyValueScalarItem>();
  target->SetAnyTypeName(sup::dto::kInt32TypeName);
  target->SetData(0);
  EXPECT_EQ(target->Data<int>(), 0);

  mock_listener_t listener(&model);

  EXPECT_CALL(listener, OnEvent(_)).Times(1);

  EXPECT_NO_THROW(UpdateAnyValueItemData(*source, *target));
  EXPECT_EQ(target->Data<int>(), 42);
}

//! Testing signaling while updating the data with UpdateAnyValueItemData method using the same
//! data.

TEST_F(AnyValueItemUtilsTest, SignalingWhileUpdatingAnyValueItemDataFromSameScalar)
{
  mvvm::ApplicationModel model;
  auto source = model.InsertItem<AnyValueScalarItem>();
  source->SetAnyTypeName(sup::dto::kInt32TypeName);
  source->SetData(42);

  auto target = model.InsertItem<AnyValueScalarItem>();
  target->SetAnyTypeName(sup::dto::kInt32TypeName);
  target->SetData(42);

  mock_listener_t listener(&model);

  EXPECT_CALL(listener, OnEvent(_)).Times(0);

  EXPECT_NO_THROW(UpdateAnyValueItemData(*source, *target));
  EXPECT_EQ(target->Data<int>(), 42);
}

//! Testing UpdateAnyValueItemData method.
//! Attempt to update structure from the source with different layout.

TEST_F(AnyValueItemUtilsTest, UpdateAnyValueItemDataFromDifferentStructs)
{
  AnyValueStructItem source;
  source.AddScalarField("signed", sup::dto::kInt32TypeName, 42);

  AnyValueStructItem target;

  EXPECT_THROW(UpdateAnyValueItemData(source, target), RuntimeException);
}

//! Testing UpdateAnyValueItemData method.
//! Updating one structure from another structure. Both structuers have the same layout (single
//! field with the same scalar).

TEST_F(AnyValueItemUtilsTest, UpdateAnyValueItemDataFromStructWithSingleField)
{
  AnyValueStructItem source;
  source.AddScalarField("signed", sup::dto::kInt32TypeName, 42);

  AnyValueStructItem target;
  auto scalar1 = target.AddScalarField("signed", sup::dto::kInt32TypeName, 43);

  EXPECT_NO_THROW(UpdateAnyValueItemData(source, target));
  EXPECT_EQ(scalar1->Data<int>(), 42);
}

TEST_F(AnyValueItemUtilsTest, UpdateAnyValueItemDataFromStructWithNestedField)
{
  AnyValueStructItem source;
  auto source_inner_struct = source.InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  source_inner_struct->SetDisplayName("struct0");
  source_inner_struct->AddScalarField("value", sup::dto::kInt32TypeName, 42);
  source_inner_struct->AddScalarField("flag", sup::dto::kBooleanTypeName, true);

  AnyValueStructItem target;
  auto targets_inner_struct = target.InsertItem<AnyValueStructItem>(mvvm::TagIndex::Append());
  targets_inner_struct->SetDisplayName("struct0");
  auto target_scalar0 = targets_inner_struct->AddScalarField("value", sup::dto::kInt32TypeName, 0);
  auto target_scalar1 =
      targets_inner_struct->AddScalarField("flag", sup::dto::kBooleanTypeName, false);

  EXPECT_NO_THROW(UpdateAnyValueItemData(source, target));
  EXPECT_EQ(target_scalar0->Data<int>(), 42);
  EXPECT_EQ(target_scalar1->Data<bool>(), true);
}

TEST_F(AnyValueItemUtilsTest, IsSuitableScalarType)
{
  {  // empty array
    AnyValueArrayItem item;
    EXPECT_TRUE(IsSuitableScalarType(item, sup::dto::kInt32TypeName));
  }

  {  // array with matching type
    AnyValueArrayItem item;
    item.InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append())
        ->SetAnyTypeName(sup::dto::kInt32TypeName);
    EXPECT_TRUE(IsSuitableScalarType(item, sup::dto::kInt32TypeName));
    EXPECT_FALSE(IsSuitableScalarType(item, sup::dto::kInt16TypeName));
  }

  {  // array with wrong type
    AnyValueArrayItem item;
    item.InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append())
        ->SetAnyTypeName(sup::dto::kInt32TypeName);
    item.InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append())
        ->SetAnyTypeName(sup::dto::kInt32TypeName);
    EXPECT_FALSE(IsSuitableScalarType(item, sup::dto::kInt16TypeName));
  }
}

TEST_F(AnyValueItemUtilsTest, GetAnyValueItemTypes)
{
  const auto types = GetAnyValueItemTypes();

  EXPECT_EQ(types.size(), 4);

  EXPECT_NE(std::find(types.begin(), types.end(), AnyValueEmptyItem::Type), types.end());
  EXPECT_NE(std::find(types.begin(), types.end(), AnyValueScalarItem::Type), types.end());
  EXPECT_NE(std::find(types.begin(), types.end(), AnyValueStructItem::Type), types.end());
  EXPECT_NE(std::find(types.begin(), types.end(), AnyValueArrayItem::Type), types.end());

  EXPECT_EQ(std::find(types.begin(), types.end(), AnyValueItem::Type), types.end());
}

TEST_F(AnyValueItemUtilsTest, CreateAnyValueTag)
{
  auto tag = CreateAnyValueTag("tag");

  EXPECT_TRUE(tag.IsValidType(AnyValueEmptyItem::Type));
  EXPECT_TRUE(tag.IsValidType(AnyValueScalarItem::Type));
  EXPECT_TRUE(tag.IsValidType(AnyValueStructItem::Type));
  EXPECT_TRUE(tag.IsValidType(AnyValueArrayItem::Type));

  EXPECT_FALSE(tag.IsValidType(AnyValueItem::Type));
  EXPECT_FALSE(tag.IsValidType(mvvm::SessionItem::Type));
}
