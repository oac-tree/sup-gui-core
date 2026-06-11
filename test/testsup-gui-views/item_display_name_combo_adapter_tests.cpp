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

#include "sup/gui/components/item_display_name_combo_adapter.h"

#include <sup/gui/model/configuration_item.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for ItemDisplayNameComboAdapter class.
 */
class ItemDisplayNameComboAdapterTest : public ::testing::Test
{
};

TEST_F(ItemDisplayNameComboAdapterTest, EmptyList)
{
  ItemDisplayNameComboAdapter adapter(std::vector<mvvm::SessionItem*>{});

  const ConfigurationItem item;

  EXPECT_EQ(adapter.GetAvailableItemNames(),
            QList<QString>({QString(ItemDisplayNameComboAdapter::kNoSelection)}));
  EXPECT_EQ(adapter.GetItemFromComboIndex(-1), nullptr);
  EXPECT_EQ(adapter.GetItemFromComboIndex(0), nullptr);
  EXPECT_EQ(adapter.GetItemFromComboIndex(1), nullptr);
  EXPECT_EQ(adapter.GetComboIndexFromItem(&item), 0);
}

TEST_F(ItemDisplayNameComboAdapterTest, ThreeConfigItems)
{
  ConfigurationItem item1;
  item1.SetDisplayName("First");
  ConfigurationItem item2;
  item2.SetDisplayName("Second");
  ConfigurationItem item3;
  item3.SetDisplayName("Third");

  ItemDisplayNameComboAdapter adapter(std::vector<mvvm::SessionItem*>{&item1, &item2, &item3});

  EXPECT_EQ(adapter.GetAvailableItemNames(),
            QList<QString>({QString(ItemDisplayNameComboAdapter::kNoSelection), QString("First"),
                            QString("Second"), QString("Third")}));
  EXPECT_EQ(adapter.GetItemFromComboIndex(-1), nullptr);

  EXPECT_EQ(adapter.GetItemFromComboIndex(0), nullptr);
  EXPECT_EQ(adapter.GetItemFromComboIndex(1), &item1);
  EXPECT_EQ(adapter.GetItemFromComboIndex(2), &item2);
  EXPECT_EQ(adapter.GetItemFromComboIndex(3), &item3);

  EXPECT_EQ(adapter.GetItemFromComboIndex(4), nullptr);
}

}  // namespace sup::gui::test
