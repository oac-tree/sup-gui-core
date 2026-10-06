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

#include "sup/gui/widgets/collapsible_list_view.h"

#include <sup/gui/settings/in_memory_settings_storage.h>
#include <sup/gui/settings/settings_manager.h>
#include <sup/gui/widgets/settings_splitter.h>

#include <gtest/gtest.h>

#include <QSplitter>

namespace sup::gui::test
{

namespace
{

constexpr auto kGroup = "View/collapsible_list";

}  // namespace

/**
 * @brief Tests for CollapsibleListView class.
 */
class CollapsibleListViewTest : public ::testing::Test
{
public:
  CollapsibleListViewTest() { m_manager.SetStorage(SettingsScope::kApplication, &m_storage); }

  InMemorySettingsStorage m_storage;
  SettingsManager m_manager;
};

TEST_F(CollapsibleListViewTest, SettingsSplitter)
{
  const CollapsibleListView view(kGroup, m_manager);

  auto splitter = dynamic_cast<SettingsSplitter*>(view.GetSplitter());
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->orientation(), Qt::Vertical);
}

TEST_F(CollapsibleListViewTest, CollapsedWidgetIsRestored)
{
  {
    CollapsibleListView view(kGroup, m_manager);
    view.AddCollapsibleWidget(new QWidget, {});
    view.AddCollapsibleWidget(new QWidget, {}, /*set_initially_expanded*/ false);
    view.ReadSettings();
    m_manager.Collect(SettingsScope::kApplication);
  }

  CollapsibleListView view(kGroup, m_manager);
  auto widget0 = new QWidget;
  auto widget1 = new QWidget;
  view.AddCollapsibleWidget(widget0, {});
  view.AddCollapsibleWidget(widget1, {});
  view.ReadSettings();

  const auto info = m_manager.Get<SplitterInfo>(kGroup, SettingsScope::kApplication);
  ASSERT_EQ(info.visibility.size(), 4);
  EXPECT_EQ(info.visibility, std::vector<std::int32_t>({1, 1, 1, 0}));
  EXPECT_TRUE(view.GetSplitter()->widget(3)->isHidden());
}

}  // namespace sup::gui::test
