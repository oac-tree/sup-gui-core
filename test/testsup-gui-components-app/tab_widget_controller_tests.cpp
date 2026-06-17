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

#include "sup/gui/component_objects/tab_widget_controller.h"

#include <gtest/gtest.h>

#include <QLabel>
#include <QTabWidget>
#include <memory>

namespace sup::gui::test
{

class TabWidgetControllerTest : public ::testing::Test
{
protected:
  //! Populates a tab widget with the given number of pages.
  static void Populate(QTabWidget& tab_widget, int page_count)
  {
    for (int i = 0; i < page_count; ++i)
    {
      tab_widget.addTab(new QLabel(QString("page%1").arg(i)), QString("tab%1").arg(i));
    }
  }

  //! Convenience: create two tab widgets with identical pages and register them.
  void SetupTwoTabWidgets(int page_count = 3)
  {
    Populate(m_tab1, page_count);
    Populate(m_tab2, page_count);
    m_controller.AddTabWidget(&m_tab1);
    m_controller.AddTabWidget(&m_tab2);
  }

  TabWidgetController m_controller;
  QTabWidget m_tab1;
  QTabWidget m_tab2;
};

//! Controller starts active with no registered widgets.

TEST_F(TabWidgetControllerTest, InitialState)
{
  EXPECT_TRUE(m_controller.IsActive());
}

//! Switching the current tab in tab1 mirrors to tab2.

TEST_F(TabWidgetControllerTest, MirrorCurrentTab)
{
  SetupTwoTabWidgets();

  m_tab1.setCurrentIndex(2);

  EXPECT_EQ(m_tab1.currentIndex(), 2);
  EXPECT_EQ(m_tab2.currentIndex(), 2);
}

//! Mirroring is bidirectional: changing tab2 updates tab1.

TEST_F(TabWidgetControllerTest, MirrorIsBidirectional)
{
  SetupTwoTabWidgets();

  m_tab2.setCurrentIndex(1);

  EXPECT_EQ(m_tab1.currentIndex(), 1);
  EXPECT_EQ(m_tab2.currentIndex(), 1);
}

//! SetActive(false) suspends mirroring; SetActive(true) re-enables it.

TEST_F(TabWidgetControllerTest, SetActiveDisablesMirroring)
{
  SetupTwoTabWidgets();

  m_controller.SetActive(false);
  EXPECT_FALSE(m_controller.IsActive());

  m_tab1.setCurrentIndex(2);
  EXPECT_EQ(m_tab1.currentIndex(), 2);
  EXPECT_EQ(m_tab2.currentIndex(), 0);  // not mirrored

  m_controller.SetActive(true);
  m_tab1.setCurrentIndex(1);
  EXPECT_EQ(m_tab2.currentIndex(), 1);  // mirroring resumed
}

//! Re-enabling synchronisation realigns all widgets to the first widget's current tab.

TEST_F(TabWidgetControllerTest, SetActiveRealignsToFirstWidget)
{
  SetupTwoTabWidgets();

  m_controller.SetActive(false);

  // while disabled, widgets diverge
  m_tab1.setCurrentIndex(2);
  m_tab2.setCurrentIndex(1);
  EXPECT_EQ(m_tab1.currentIndex(), 2);
  EXPECT_EQ(m_tab2.currentIndex(), 1);

  // re-enabling should align all widgets to the first widget's tab
  m_controller.SetActive(true);
  EXPECT_EQ(m_tab1.currentIndex(), 2);
  EXPECT_EQ(m_tab2.currentIndex(), 2);
}

//! Re-enabling synchronisation with no registered widgets does not crash.

TEST_F(TabWidgetControllerTest, SetActiveWithoutWidgets)
{
  m_controller.SetActive(false);
  EXPECT_NO_THROW(m_controller.SetActive(true));
}

//! Calling SetActive(true) while already active should not realign widgets.

TEST_F(TabWidgetControllerTest, SetActiveWhenAlreadyActive)
{
  SetupTwoTabWidgets();

  // diverge directly via the controller being active is not possible, so set up manually:
  // here both widgets are active and aligned at 0.
  EXPECT_TRUE(m_controller.IsActive());

  // a redundant activation must be a no-op (no realignment to first widget)
  m_tab2.blockSignals(true);
  m_tab2.setCurrentIndex(1);
  m_tab2.blockSignals(false);

  m_controller.SetActive(true);  // already active, should not touch widgets
  EXPECT_EQ(m_tab1.currentIndex(), 0);
  EXPECT_EQ(m_tab2.currentIndex(), 1);
}

//! RemoveTabWidget stops mirroring to the removed widget.

TEST_F(TabWidgetControllerTest, RemoveTabWidget)
{
  SetupTwoTabWidgets();

  m_controller.RemoveTabWidget(&m_tab2);

  m_tab1.setCurrentIndex(2);
  EXPECT_EQ(m_tab1.currentIndex(), 2);
  EXPECT_EQ(m_tab2.currentIndex(), 0);  // no longer mirrored
}

//! Adding the same widget twice does not cause double registration.

TEST_F(TabWidgetControllerTest, AddDuplicateWidgetIsIgnored)
{
  SetupTwoTabWidgets();
  m_controller.AddTabWidget(&m_tab1);  // duplicate — should be ignored

  m_tab1.setCurrentIndex(1);
  EXPECT_EQ(m_tab2.currentIndex(), 1);
}

//! Switching to an index that does not exist in another widget is skipped safely.

TEST_F(TabWidgetControllerTest, IndexOutOfRangeIsSkipped)
{
  Populate(m_tab1, 4);
  Populate(m_tab2, 2);  // fewer pages
  m_controller.AddTabWidget(&m_tab1);
  m_controller.AddTabWidget(&m_tab2);

  // index 3 does not exist in tab2; mirroring must not crash or change tab2.
  m_tab1.setCurrentIndex(3);
  EXPECT_EQ(m_tab1.currentIndex(), 3);
  EXPECT_EQ(m_tab2.currentIndex(), 0);

  // index 1 exists in both.
  m_tab1.setCurrentIndex(1);
  EXPECT_EQ(m_tab2.currentIndex(), 1);
}

//! Destroying a registered widget removes it automatically; no crash on subsequent switch.

TEST_F(TabWidgetControllerTest, DestroyedWidgetIsRemovedAutomatically)
{
  auto tab3 = std::make_unique<QTabWidget>();
  Populate(*tab3, 3);
  m_controller.AddTabWidget(tab3.get());

  SetupTwoTabWidgets();

  tab3.reset();  // destroy — controller must clean up its pointer

  m_tab1.setCurrentIndex(2);
  EXPECT_EQ(m_tab2.currentIndex(), 2);
}

}  // namespace sup::gui::test
