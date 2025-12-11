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

#include "sup/gui/views/waveformeditor/anyvalue_waveform_editor.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for AnyValueWaveformEditor class.
 */
class AnyValueWaveformEditorTest : public ::testing::Test
{
};

TEST_F(AnyValueWaveformEditorTest, InitialState)
{
  AnyValueWaveformEditor editor;

  auto waveform_item = editor.GetResult();
  ASSERT_NE(waveform_item.get(), nullptr);

  ASSERT_TRUE(waveform_item->IsArray());
  EXPECT_EQ(waveform_item->GetChildrenCount(), 0U);
}

TEST_F(AnyValueWaveformEditorTest, CreateWaveformFromWrongItem)
{
  AnyValueWaveformEditor editor;

  AnyValueScalarItem wrong_item;

  editor.SetInitialValue(&wrong_item);

  auto waveform_item = editor.GetResult();
  ASSERT_NE(waveform_item.get(), nullptr);

  ASSERT_TRUE(waveform_item->IsArray());
  EXPECT_EQ(waveform_item->GetChildrenCount(), 0U);
}

TEST_F(AnyValueWaveformEditorTest, CreateWaveformFromArrayItem)
{
  AnyValueWaveformEditor editor;
  auto array_item = std::make_unique<AnyValueArrayItem>();
  auto struct_item0 = std::make_unique<AnyValueStructItem>();
  struct_item0->AddScalarField("a", constants::kFloat64TypeName, 1.0);
  struct_item0->AddScalarField("b", constants::kFloat64TypeName, 10.0);
  auto struct_item1 = std::make_unique<AnyValueStructItem>();
  struct_item1->AddScalarField("a", constants::kFloat64TypeName, 2.0);
  struct_item1->AddScalarField("b", constants::kFloat64TypeName, 20.0);
  auto struct_item2 = std::make_unique<AnyValueStructItem>();
  struct_item2->AddScalarField("a", constants::kFloat64TypeName, 3.0);
  struct_item2->AddScalarField("b", constants::kFloat64TypeName, 30.0);
  array_item->InsertItem(std::move(struct_item0), mvvm::TagIndex::Append());
  array_item->InsertItem(std::move(struct_item1), mvvm::TagIndex::Append());
  array_item->InsertItem(std::move(struct_item2), mvvm::TagIndex::Append());
  editor.SetInitialValue(array_item.get());

  // getting back edited waveform
  auto waveform_item = editor.GetResult();
  ASSERT_NE(waveform_item.get(), nullptr);
  ASSERT_TRUE(waveform_item->IsArray());
  EXPECT_EQ(waveform_item->GetChildrenCount(), 3U);

  auto structs = waveform_item->GetChildren();
  ASSERT_EQ(structs.size(), 3);
  auto fields = structs.at(1)->GetChildren();  // middle point
  EXPECT_EQ(fields.at(0)->Data<double>(), 2.0);
  EXPECT_EQ(fields.at(1)->Data<double>(), 20.0);
}

}  // namespace sup::gui::test
