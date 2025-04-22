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

#include "sup/gui/components/anyvalue_editor_helper.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

namespace
{
const std::string kUndefined("undefined");
}

//! Testing utility methods in anyvalue_editor_helper.h
class AnyValueEditorHelperTest : public ::testing::Test
{
};

TEST_F(AnyValueEditorHelperTest, HasOneOfDefaultDisplayNames)
{
  {
    const AnyValueStructItem item;
    EXPECT_TRUE(HasOneOfDefaultDisplayNames(item));
  }
  {
    AnyValueArrayItem item;
    EXPECT_TRUE(HasOneOfDefaultDisplayNames(item));

    item.SetDisplayName(constants::kAnyValueDefaultDisplayName);
    EXPECT_TRUE(HasOneOfDefaultDisplayNames(item));

    item.SetDisplayName("abc");
    EXPECT_FALSE(HasOneOfDefaultDisplayNames(item));
  }
  {
    const AnyValueScalarItem item;
    EXPECT_TRUE(HasOneOfDefaultDisplayNames(item));
  }
  {
    const AnyValueEmptyItem item;
    EXPECT_TRUE(HasOneOfDefaultDisplayNames(item));
  }
}

//! Simulating case when AnyValueItem is a top-level item.
TEST_F(AnyValueEditorHelperTest, SuggestDisplayNameForTopLevelItem)
{
  const mvvm::ContainerItem container;

  {  // empty
    const AnyValueEmptyItem item;
    auto name = SuggestDisplayName(container, item);
    EXPECT_EQ(name.value_or(kUndefined), constants::kAnyValueDefaultDisplayName);
  }

  {  // scalar
    const AnyValueScalarItem item;
    auto name = SuggestDisplayName(container, item);
    EXPECT_EQ(name.value_or(kUndefined), constants::kAnyValueDefaultDisplayName);
  }

  {  // struct
    const AnyValueStructItem item;
    auto name = SuggestDisplayName(container, item);
    EXPECT_EQ(name.value_or(kUndefined), constants::kAnyValueDefaultDisplayName);
  }

  {  // array
    const AnyValueArrayItem item;
    auto name = SuggestDisplayName(container, item);
    EXPECT_EQ(name.value_or(kUndefined), constants::kAnyValueDefaultDisplayName);
  }
}

TEST_F(AnyValueEditorHelperTest, SuggestNameForStructField)
{
  {
    const AnyValueStructItem parent;
    const AnyValueScalarItem child;

    auto name = SuggestDisplayName(parent, child);
    EXPECT_EQ(name.value_or(kUndefined), constants::kFieldNamePrefix + "0");
  }

  {
    // existing original name will be used as a field name
    const AnyValueStructItem parent;
    AnyValueScalarItem child;
    child.SetDisplayName("width");

    auto name = SuggestDisplayName(parent, child);
    EXPECT_EQ(name.value_or(kUndefined), kUndefined);
  }
}

TEST_F(AnyValueEditorHelperTest, SuggestNameForArrayElement)
{
  const AnyValueArrayItem parent;
  const AnyValueScalarItem child;

  auto name = SuggestDisplayName(parent, child);
  EXPECT_EQ(name.value_or(kUndefined), constants::kElementNamePrefix + "0");
}

}  // namespace sup::gui::test
