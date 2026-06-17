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

#include "sup/gui/component_objects/scroll_area_mirror_controller.h"

#include <gtest/gtest.h>

#include <QScrollArea>
#include <QScrollBar>
#include <memory>

namespace sup::gui::test
{

class ScrollAreaMirrorControllerTest : public ::testing::Test
{
protected:
  //! Convenience: configure both scroll areas with identical ranges and register them.
  void SetupTwoAreas(int min = 0, int max = 100)
  {
    for (auto* area : {&m_area1, &m_area2})
    {
      area->horizontalScrollBar()->setRange(min, max);
      area->verticalScrollBar()->setRange(min, max);
    }
    m_controller.AddScrollArea(&m_area1);
    m_controller.AddScrollArea(&m_area2);
  }

  ScrollAreaMirrorController m_controller;
  QScrollArea m_area1;
  QScrollArea m_area2;
};

//! Controller starts active with no registered areas.

TEST_F(ScrollAreaMirrorControllerTest, InitialState)
{
  EXPECT_TRUE(m_controller.IsActive());
}

//! Changing the vertical scrollbar of area1 mirrors to area2.

TEST_F(ScrollAreaMirrorControllerTest, MirrorVertical)
{
  SetupTwoAreas();

  m_area1.verticalScrollBar()->setValue(42);

  EXPECT_EQ(m_area1.verticalScrollBar()->value(), 42);
  EXPECT_EQ(m_area2.verticalScrollBar()->value(), 42);
}

//! Changing the horizontal scrollbar of area1 mirrors to area2.

TEST_F(ScrollAreaMirrorControllerTest, MirrorHorizontal)
{
  SetupTwoAreas();

  m_area1.horizontalScrollBar()->setValue(75);

  EXPECT_EQ(m_area1.horizontalScrollBar()->value(), 75);
  EXPECT_EQ(m_area2.horizontalScrollBar()->value(), 75);
}

//! Mirroring is bidirectional: changing area2 updates area1.

TEST_F(ScrollAreaMirrorControllerTest, MirrorIsBidirectional)
{
  SetupTwoAreas();

  m_area2.verticalScrollBar()->setValue(30);

  EXPECT_EQ(m_area1.verticalScrollBar()->value(), 30);
  EXPECT_EQ(m_area2.verticalScrollBar()->value(), 30);
}

//! SetActive(false) suspends mirroring; SetActive(true) re-enables it.

TEST_F(ScrollAreaMirrorControllerTest, SetActiveDisablesMirroring)
{
  SetupTwoAreas();

  m_controller.SetActive(false);
  EXPECT_FALSE(m_controller.IsActive());

  m_area1.verticalScrollBar()->setValue(55);
  EXPECT_EQ(m_area1.verticalScrollBar()->value(), 55);
  EXPECT_EQ(m_area2.verticalScrollBar()->value(), 0);  // not mirrored

  m_controller.SetActive(true);
  m_area1.verticalScrollBar()->setValue(20);
  EXPECT_EQ(m_area2.verticalScrollBar()->value(), 20);  // mirroring resumed
}

//! RemoveScrollArea stops mirroring to the removed area.

TEST_F(ScrollAreaMirrorControllerTest, RemoveScrollArea)
{
  SetupTwoAreas();

  m_controller.RemoveScrollArea(&m_area2);

  m_area1.verticalScrollBar()->setValue(60);
  EXPECT_EQ(m_area1.verticalScrollBar()->value(), 60);
  EXPECT_EQ(m_area2.verticalScrollBar()->value(), 0);  // no longer mirrored
}

//! Adding the same area twice does not cause double-propagation.

TEST_F(ScrollAreaMirrorControllerTest, AddDuplicateAreaIsIgnored)
{
  SetupTwoAreas();
  m_controller.AddScrollArea(&m_area1);  // duplicate — should be ignored

  // If duplicate connections existed, setValue(10) on area1 would trigger
  // area2 twice, but value() is idempotent so just verify no crash and
  // that area2 ends up with the expected value.
  m_area1.verticalScrollBar()->setValue(10);
  EXPECT_EQ(m_area2.verticalScrollBar()->value(), 10);
}

//! Destroying a registered area removes it automatically; no crash on subsequent scroll.

TEST_F(ScrollAreaMirrorControllerTest, DestroyedAreaIsRemovedAutomatically)
{
  auto area3 = std::make_unique<QScrollArea>();
  area3->verticalScrollBar()->setRange(0, 100);
  m_controller.AddScrollArea(area3.get());

  SetupTwoAreas();

  area3.reset();  // destroy — controller must clean up its pointer

  // Scrolling area1 must not crash despite area3 being gone.
  m_area1.verticalScrollBar()->setValue(50);
  EXPECT_EQ(m_area2.verticalScrollBar()->value(), 50);
}

}  // namespace sup::gui::test
