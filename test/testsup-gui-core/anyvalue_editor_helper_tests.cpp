/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sup/gui/components/anyvalue_editor_helper.h"

#include <sup/gui/model/anyvalue_item.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

using namespace sup::gui;

namespace
{
const std::string kUndefined("undefined");
}

//! Testing utility methods in anyvalue_editor_helper.h

class AnyValueEditorHelperTest : public ::testing::Test
{
};

//! Simulating case when AnyValueItem is a top-level item.

TEST_F(AnyValueEditorHelperTest, SuggestDisplayNameForTopLevelItem)
{
  mvvm::ContainerItem container;

  { // empty
    AnyValueEmptyItem item;
    auto name = SuggestDisplayName(container, item);
    EXPECT_EQ(name.value_or(kUndefined), kAnyValueDefaultDisplayName);
  }

  { // scalar
    AnyValueScalarItem item;
    auto name = SuggestDisplayName(container, item);
    EXPECT_EQ(name.value_or(kUndefined), kAnyValueDefaultDisplayName);
  }

  { // struct
    AnyValueStructItem item;
    auto name = SuggestDisplayName(container, item);
    EXPECT_EQ(name.value_or(kUndefined), kAnyValueDefaultDisplayName);
  }

  { // array
    AnyValueArrayItem item;
    auto name = SuggestDisplayName(container, item);
    EXPECT_EQ(name.value_or(kUndefined), kAnyValueDefaultDisplayName);
  }
}

TEST_F(AnyValueEditorHelperTest, SuggestNameForStructField)
{
  AnyValueStructItem parent;
  AnyValueScalarItem child;

  auto name = SuggestDisplayName(parent, child);
  EXPECT_EQ(name.value_or(kUndefined), kFieldNamePrefix + "0");
}

TEST_F(AnyValueEditorHelperTest, SuggestNameForArrayElement)
{
  AnyValueArrayItem parent;
  AnyValueScalarItem child;

  auto name = SuggestDisplayName(parent, child);
  EXPECT_EQ(name.value_or(kUndefined), kElementNamePrefix + "0");
}
