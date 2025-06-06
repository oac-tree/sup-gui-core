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

#include "sup/gui/components/anyvalue_item_copy_helper.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

#include <QMimeData>

namespace sup::gui::test
{

/**
 * @brief Tests for helper methods in anyvalue_
 */
class AnyValueItemCopyHelperTest : public ::testing::Test
{
public:
  /**
   * @brief Helper function to take vector of pointers casted to certain type from the vector of
   * unique_ptr.
   */
  template <typename ContainerT, typename InstructionT = AnyValueItem>
  std::vector<InstructionT*> GetItemsPtr(const ContainerT& vector_of_unique_ptr)
  {
    auto vector_of_pointers = mvvm::utils::GetVectorOfPtrs(vector_of_unique_ptr);
    return mvvm::utils::CastItems<InstructionT>(vector_of_pointers);
  }
};

TEST_F(AnyValueItemCopyHelperTest, CreateAnyValueItemSelectionCopyMimeData)
{
  mvvm::ApplicationModel model;

  // parent and two children
  auto struct0 = model.InsertItem<AnyValueStructItem>();
  struct0->SetAnyTypeName("struct0");

  // children
  auto field0 = struct0->AddScalarField("field0", sup::dto::kInt32TypeName, mvvm::int32{42});
  auto field1 = struct0->AddScalarField("field1", sup::dto::kInt32TypeName, mvvm::int32{43});
  auto struct1 = model.InsertItem<AnyValueStructItem>(struct0);
  struct1->SetAnyTypeName("struct1");

  // grandchildren
  auto field2 = struct1->AddScalarField("field2", sup::dto::kInt32TypeName, mvvm::int32{44});

  {  // user selection contains two children only
    const std::vector<AnyValueItem*> selection({field0, struct1});

    const auto mime_data = CreateAnyValueItemSelectionCopyMimeData(selection);

    auto reconstructed_items = CreateAnyValueItems(mime_data.get());
    auto reconstructed_anyvalues = GetItemsPtr(reconstructed_items);

    ASSERT_EQ(reconstructed_anyvalues.size(), 2);
    EXPECT_EQ(reconstructed_anyvalues.at(0)->GetDisplayName(), std::string("field0"));
    EXPECT_EQ(reconstructed_anyvalues.at(1)->GetAnyTypeName(), std::string("struct1"));

    // sequence doesn't contain any childre
    EXPECT_TRUE(reconstructed_anyvalues.at(1)->GetChildren().empty());
  }

  {  // user selection contains main sequence
    const std::vector<AnyValueItem*> selection({struct0, struct1, field2});

    const auto mime_data = CreateAnyValueItemSelectionCopyMimeData(selection);

    auto reconstructed_items = CreateAnyValueItems(mime_data.get());
    auto reconstructed_anyvalues = GetItemsPtr(reconstructed_items);

    ASSERT_EQ(reconstructed_anyvalues.size(), 1);
    auto reconstructed_parent = reconstructed_anyvalues.at(0);
    EXPECT_EQ(reconstructed_parent->GetAnyTypeName(), std::string("struct0"));

    // checking child
    ASSERT_EQ(reconstructed_parent->GetChildren().size(), 1);
    auto reconstructed_child = reconstructed_parent->GetChildren().at(0);
    EXPECT_EQ(reconstructed_child->GetAnyTypeName(), std::string("struct1"));

    // checking grandchild
    ASSERT_EQ(reconstructed_child->GetChildren().size(), 1);
    auto reconstructed_grandchild = reconstructed_child->GetChildren().at(0);
    EXPECT_EQ(reconstructed_grandchild->GetDisplayName(), std::string("field2"));
    EXPECT_EQ(reconstructed_grandchild->Data<mvvm::int32>(), 44);
  }
}

}  // namespace sup::gui::test
