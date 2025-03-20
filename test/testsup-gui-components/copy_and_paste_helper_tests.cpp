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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sup/gui/components/copy_and_paste_helper.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_model.h>

#include <gtest/gtest.h>

using namespace sup::gui;

class CopyAndPasteHelperTest : public ::testing::Test
{
};

TEST_F(CopyAndPasteHelperTest, GetTopLevelSelection)
{
  EXPECT_TRUE(GetTopLevelSelection(std::vector<mvvm::SessionItem*>()).empty());

  // item without parent
  mvvm::SessionItem item;
  const std::vector<mvvm::SessionItem*> expected({&item});
  EXPECT_EQ(GetTopLevelSelection(expected), expected);
}

TEST_F(CopyAndPasteHelperTest, GetTopLevelSelectionTwoItems)
{
  mvvm::SessionModel model;
  auto item0 = model.InsertItem<mvvm::PropertyItem>();
  auto item1 = model.InsertItem<mvvm::PropertyItem>();
  const std::vector<mvvm::SessionItem*> expected({item0, item1});
  EXPECT_EQ(GetTopLevelSelection(expected), expected);
}

TEST_F(CopyAndPasteHelperTest, GetTopLevelSelectionTwoItemsWithChildren)
{
  mvvm::SessionModel model;
  auto item0 = model.InsertItem<mvvm::PropertyItem>();
  auto item1 = model.InsertItem<mvvm::PropertyItem>();
  const std::vector<mvvm::SessionItem*> expected({item0, item1});
  EXPECT_EQ(GetTopLevelSelection(expected), expected);
}

TEST_F(CopyAndPasteHelperTest, GetTopLevelSelectionAnyValueStructWithChildren)
{
  mvvm::SessionModel model;

  auto struct0 = model.InsertItem<AnyValueStructItem>();
  auto scalar1 = model.InsertItem<AnyValueScalarItem>(struct0);
  auto scalar2 = model.InsertItem<AnyValueScalarItem>(struct0);

  const std::vector<mvvm::SessionItem*> selection({struct0, scalar1, scalar2});
  const std::vector<mvvm::SessionItem*> expected({struct0});
  EXPECT_EQ(GetTopLevelSelection(selection), expected);
}

//! Reproducing example from comments to the GetTopLevelSelection function
TEST_F(CopyAndPasteHelperTest, GetTopLevelSelectionAnyValueTwoStructWithChildren)
{
  mvvm::SessionModel model;

  auto parent0 = model.InsertItem<AnyValueStructItem>();
  auto child0 = model.InsertItem<AnyValueScalarItem>(parent0);

  auto parent1 = model.InsertItem<AnyValueStructItem>();
  auto child1 = model.InsertItem<AnyValueScalarItem>(parent1);
  auto child2 = model.InsertItem<AnyValueScalarItem>(parent1);

  auto parent2 = model.InsertItem<AnyValueStructItem>();
  auto child3 = model.InsertItem<AnyValueScalarItem>(parent2);

  auto parent3 = model.InsertItem<AnyValueStructItem>();

  const std::vector<mvvm::SessionItem*> selection(
      {child0, parent1, child1, parent2, child3, parent3});
  const std::vector<mvvm::SessionItem*> expected({parent1, parent2, parent3});
  EXPECT_EQ(GetTopLevelSelection(selection), expected);
}

TEST_F(CopyAndPasteHelperTest, FilterOutChildren)
{
  mvvm::SessionModel model;

  auto parent0 = model.InsertItem<AnyValueStructItem>();
  auto child0 = model.InsertItem<AnyValueScalarItem>(parent0);

  auto parent1 = model.InsertItem<AnyValueStructItem>();
  auto child1 = model.InsertItem<AnyValueScalarItem>(parent1);
  auto child2 = model.InsertItem<AnyValueScalarItem>(parent1);

  auto parent2 = model.InsertItem<AnyValueStructItem>();
  auto child3 = model.InsertItem<AnyValueScalarItem>(parent2);

  auto parent3 = model.InsertItem<AnyValueStructItem>();

  {
    const std::vector<mvvm::SessionItem*> selection;
    EXPECT_TRUE(FilterOutChildren(selection).empty());
  }

  {
    const std::vector<mvvm::SessionItem*> selection({child0});
    const std::vector<mvvm::SessionItem*> expected({child0});
    EXPECT_EQ(FilterOutChildren(selection), expected);
  }

  {
    const std::vector<mvvm::SessionItem*> selection({child0, parent0});
    const std::vector<mvvm::SessionItem*> expected({parent0});
    EXPECT_EQ(FilterOutChildren(selection), expected);
  }

  {
    const std::vector<mvvm::SessionItem*> selection({parent0, child0});
    const std::vector<mvvm::SessionItem*> expected({parent0});
    EXPECT_EQ(FilterOutChildren(selection), expected);
  }

  {
    const std::vector<mvvm::SessionItem*> selection(
        {child0, parent1, child1, parent2, child3, parent3});
    const std::vector<mvvm::SessionItem*> expected({child0, parent1, parent2, parent3});
    EXPECT_EQ(FilterOutChildren(selection), expected);
  }
}
