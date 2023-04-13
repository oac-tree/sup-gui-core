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

#include "sup/gui/anyvalueeditor/anyvalue_editor_helper.h"

#include <sup/gui/model/anyvalue_item.h>

#include <gtest/gtest.h>

using namespace sup::gui;

//! Testing utility methods in anyvalue_editor_helper.h

class AnyValueEditorHelperTest : public ::testing::Test
{
};

TEST_F(AnyValueEditorHelperTest, SuggestNameForStructField)
{
  AnyValueStructItem parent;
  AnyValueScalarItem child;

  auto name = SuggestDisplayName(parent, child);
  ASSERT_TRUE(name.has_value());
  EXPECT_EQ(name.value(), sup::gui::kFieldNamePrefix + "0");
}

TEST_F(AnyValueEditorHelperTest, SuggestNameForRaayElement)
{
  AnyValueArrayItem parent;
  AnyValueScalarItem child;

  auto name = SuggestDisplayName(parent, child);
  ASSERT_TRUE(name.has_value());
  EXPECT_EQ(name.value(), sup::gui::kElementNamePrefix + "0");
}
