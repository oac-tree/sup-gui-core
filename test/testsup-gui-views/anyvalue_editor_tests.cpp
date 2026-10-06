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

#include "sup/gui/views/anyvalueeditor/anyvalue_editor.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <gtest/gtest.h>
#include <testutils/test_settings_manager.h>

namespace sup::gui::test
{

/**
 * @brief Tests for AnyValueEditorTest class.
 */
class AnyValueEditorTest : public ::testing::Test
{
public:
  TestSettingsManager m_settings_manager;
};

TEST_F(AnyValueEditorTest, InitialState)
{
  AnyValueEditor editor(m_settings_manager);

  auto result = editor.GetResult();
  EXPECT_EQ(result.get(), nullptr);
}

TEST_F(AnyValueEditorTest, SetNullAsInitialValue)
{
  AnyValueEditor editor(m_settings_manager);

  EXPECT_NO_FATAL_FAILURE(editor.SetInitialValue(nullptr));
  auto result = editor.GetResult();
  EXPECT_EQ(result.get(), nullptr);
}

TEST_F(AnyValueEditorTest, SetInt32AsInitialValue)
{
  AnyValueEditor editor(m_settings_manager);

  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);
  item.SetData(42);

  EXPECT_NO_FATAL_FAILURE(editor.SetInitialValue(&item));
  auto result = editor.GetResult();
  ASSERT_NE(result.get(), nullptr);
  EXPECT_TRUE(result->IsScalar());
  EXPECT_EQ(result->Data<int>(), 42);
}

}  // namespace sup::gui::test
