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

#include "sup/gui/widgets/combo_tool_bar.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <gtest/gtest.h>

#include <QMenu>
#include <QSignalSpy>

namespace sup::gui::test
{

/**
 * @brief Tests for ComboToolBar class.
 */
class ComboToolBarTest : public ::testing::Test
{
};

TEST_F(ComboToolBarTest, InitializationWithEmptyCallback)
{
  // attempt to initialize with wrong callback
  EXPECT_THROW(const ComboToolBar tool_bar({}), RuntimeException);

  // callback return empty list
  auto entries = []() -> QList<QString> { return {}; };
  ComboToolBar tool_bar(entries);
  EXPECT_EQ(tool_bar.GetCurrentIndex(), -1);
  EXPECT_TRUE(tool_bar.GetCurrentText().isEmpty());
  EXPECT_TRUE(tool_bar.GetMenu()->actions().isEmpty());

  // it ignores wrong indexes
  tool_bar.SetCurrentIndex(42);
  EXPECT_EQ(tool_bar.GetCurrentIndex(), -1);
  EXPECT_TRUE(tool_bar.GetCurrentText().isEmpty());
}

TEST_F(ComboToolBarTest, ListOfThreeItems)
{
  QList<QString> kItems{"First", "Second", "Third"};
  auto entries = [&kItems]() -> QList<QString> { return kItems; };
  ComboToolBar tool_bar(entries);

  // at construction nothing is selected
  EXPECT_EQ(tool_bar.GetCurrentIndex(), -1);
  EXPECT_TRUE(tool_bar.GetCurrentText().isEmpty());

  // menu is populated with items
  auto menu = tool_bar.GetMenu();
  ASSERT_NE(menu, nullptr);

  // make menu pop-up to populate it with actions
  menu->popup(tool_bar.mapToGlobal(tool_bar.rect().bottomLeft()));

  auto actions = menu->actions();
  ASSERT_EQ(actions.size(), kItems.size());
  for (int i = 0; i < kItems.size(); ++i)
  {
    EXPECT_EQ(actions.at(i)->text(), kItems.at(i));
  }

  // it allows to select items
  for (int i = 0; i < kItems.size(); ++i)
  {
    tool_bar.SetCurrentIndex(i);
    EXPECT_EQ(tool_bar.GetCurrentIndex(), i);
    EXPECT_EQ(tool_bar.GetCurrentText(), kItems.at(i));
  }

  tool_bar.SetCurrentIndex(0);

  // it ignores wrong indexes
  tool_bar.SetCurrentIndex(-1);
  EXPECT_EQ(tool_bar.GetCurrentIndex(), 0);
  EXPECT_EQ(tool_bar.GetCurrentText(), QString("First"));
  tool_bar.SetCurrentIndex(static_cast<std::int32_t>(kItems.size()));
}

TEST_F(ComboToolBarTest, ListOfItemsChangesOnTheNextPopUp)
{
  QList<QString> kItems{"First", "Second", "Third"};
  auto entries = [&kItems]() -> QList<QString> { return kItems; };
  ComboToolBar tool_bar(entries);

  // make menu pop-up to populate it with actions
  tool_bar.GetMenu()->popup(tool_bar.mapToGlobal(tool_bar.rect().bottomLeft()));
  auto actions = tool_bar.GetMenu()->actions();
  ASSERT_EQ(actions.size(), kItems.size());

  kItems.append("Fourth");
  // make menu pop-up again to update it with new actions
  tool_bar.GetMenu()->popup(tool_bar.mapToGlobal(tool_bar.rect().bottomLeft()));
  actions = tool_bar.GetMenu()->actions();
  ASSERT_EQ(actions.size(), kItems.size());
  for (int i = 0; i < kItems.size(); ++i)
  {
    EXPECT_EQ(actions.at(i)->text(), kItems.at(i));
  }
}

TEST_F(ComboToolBarTest, Notifications)
{
  QList<QString> kItems{"First", "Second", "Third"};
  auto entries = [&kItems]() -> QList<QString> { return kItems; };
  ComboToolBar tool_bar(entries);

  QSignalSpy select_item_spy(&tool_bar, &ComboToolBar::selectItemRequest);
  // make menu pop-up to populate it with actions
  tool_bar.GetMenu()->popup(tool_bar.mapToGlobal(tool_bar.rect().bottomLeft()));

  auto actions = tool_bar.GetMenu()->actions();

  actions.at(0)->trigger();
  ASSERT_EQ(select_item_spy.count(), 1);
  QList<QVariant> arguments = select_item_spy.takeFirst();
  EXPECT_EQ(arguments.at(0).toInt(), 0);
  EXPECT_EQ(tool_bar.GetCurrentIndex(), 0);
  EXPECT_EQ(tool_bar.GetCurrentText(), QString("First"));

  // same selection doesn't trigger notification
  actions.at(0)->trigger();
  ASSERT_EQ(select_item_spy.count(), 0);
  EXPECT_EQ(tool_bar.GetCurrentIndex(), 0);
  EXPECT_EQ(tool_bar.GetCurrentText(), QString("First"));

  actions.at(1)->trigger();
  ASSERT_EQ(select_item_spy.count(), 1);
  arguments = select_item_spy.takeFirst();
  EXPECT_EQ(arguments.at(0).toInt(), 1);
  EXPECT_EQ(tool_bar.GetCurrentIndex(), 1);
  EXPECT_EQ(tool_bar.GetCurrentText(), QString("Second"));

  // changing programmatically also triggers notification
  tool_bar.SetCurrentIndex(2);
  ASSERT_EQ(select_item_spy.count(), 1);
  arguments = select_item_spy.takeFirst();
  EXPECT_EQ(arguments.at(0).toInt(), 2);
  EXPECT_EQ(tool_bar.GetCurrentIndex(), 2);
  EXPECT_EQ(tool_bar.GetCurrentText(), QString("Third"));
}

TEST_F(ComboToolBarTest, UpdateAppearance)
{
  QList<QString> kItems{"First", "Second", "Third"};
  auto entries = [&kItems]() -> QList<QString> { return kItems; };
  ComboToolBar tool_bar(entries);

  tool_bar.SetCurrentIndex(2);

  // make menu pop-up to populate it with actions
  tool_bar.GetMenu()->popup(tool_bar.mapToGlobal(tool_bar.rect().bottomLeft()));

  EXPECT_EQ(tool_bar.GetCurrentIndex(), 2);
  EXPECT_EQ(tool_bar.GetCurrentText(), QString("Third"));

  // rename current item and update appearance
  kItems[2] = "Third (renamed)";

  tool_bar.UpdateAppearance();
  EXPECT_EQ(tool_bar.GetCurrentIndex(), 2);
  EXPECT_EQ(tool_bar.GetCurrentText(), QString("Third (renamed)"));
}

}  // namespace sup::gui::test
