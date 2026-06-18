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

#include "sup/gui/component_objects/diff_highlight_controller.h"

#include <gtest/gtest.h>

#include <QColor>
#include <QStandardItem>
#include <QStandardItemModel>
#include <memory>

namespace sup::gui::test
{

class DiffHighlightControllerTest : public ::testing::Test
{
public:
  //! Builds a two-column model with one top-level branch holding a child, plus a second top-level
  //! row. Both columns carry distinct display values.
  static std::unique_ptr<QStandardItemModel> CreateModel()
  {
    auto model = std::make_unique<QStandardItemModel>();

    auto* name0 = new QStandardItem("A");
    auto* value0 = new QStandardItem("1");
    auto* child_name = new QStandardItem("AA");
    auto* child_value = new QStandardItem("2");
    name0->appendRow({child_name, child_value});
    model->appendRow({name0, value0});

    auto* name1 = new QStandardItem("B");
    auto* value1 = new QStandardItem("3");
    model->appendRow({name1, value1});

    return model;
  }

  //! Returns true when the cell has a red background.
  static bool IsRed(const QModelIndex& index)
  {
    const auto background = index.data(Qt::BackgroundRole);
    return background.isValid() && (background.value<QColor>() == QColor(Qt::red));
  }

  //! Returns true when the cell has no background set.
  static bool HasNoBackground(const QModelIndex& index)
  {
    return !index.data(Qt::BackgroundRole).isValid();
  }
};

TEST_F(DiffHighlightControllerTest, InitialState)
{
  auto model_a = CreateModel();
  auto model_b = CreateModel();

  const DiffHighlightController controller(model_a.get(), model_b.get());
  EXPECT_TRUE(controller.IsActive());

  // no change happened yet, no highlight expected
  EXPECT_TRUE(HasNoBackground(model_b->index(0, 1)));
}

//! Changing a reference cell so it differs highlights the corresponding highlight cell.

TEST_F(DiffHighlightControllerTest, ReferenceChangeHighlightsDifference)
{
  auto model_a = CreateModel();
  auto model_b = CreateModel();

  DiffHighlightController controller(model_a.get(), model_b.get());

  model_a->setData(model_a->index(0, 1), "999", Qt::DisplayRole);

  EXPECT_TRUE(IsRed(model_b->index(0, 1)));
  // unrelated cells are untouched
  EXPECT_TRUE(HasNoBackground(model_b->index(0, 0)));
  EXPECT_TRUE(HasNoBackground(model_b->index(1, 1)));
  // the reference model itself is never highlighted
  EXPECT_TRUE(HasNoBackground(model_a->index(0, 1)));
}

//! When a previously different reference cell becomes equal again, the highlight is cleared.

TEST_F(DiffHighlightControllerTest, ReferenceChangeClearsHighlightWhenEqual)
{
  auto model_a = CreateModel();
  auto model_b = CreateModel();

  DiffHighlightController controller(model_a.get(), model_b.get());

  model_a->setData(model_a->index(0, 1), "999", Qt::DisplayRole);
  ASSERT_TRUE(IsRed(model_b->index(0, 1)));

  // restore the original value so both models match again
  model_a->setData(model_a->index(0, 1), "1", Qt::DisplayRole);
  EXPECT_TRUE(HasNoBackground(model_b->index(0, 1)));
}

//! Changing a highlight cell so it differs from the reference highlights it.

TEST_F(DiffHighlightControllerTest, HighlightChangeHighlightsDifference)
{
  auto model_a = CreateModel();
  auto model_b = CreateModel();

  DiffHighlightController controller(model_a.get(), model_b.get());

  model_b->setData(model_b->index(1, 1), "555", Qt::DisplayRole);

  EXPECT_TRUE(IsRed(model_b->index(1, 1)));
}

//! A nested cell is correctly located by its row/column path.

TEST_F(DiffHighlightControllerTest, NestedCellHighlight)
{
  auto model_a = CreateModel();
  auto model_b = CreateModel();

  DiffHighlightController controller(model_a.get(), model_b.get());

  // child value cell, located at (0, 1) under the first parent (0, 0)
  const auto parent_a = model_a->index(0, 0);
  model_a->setData(model_a->index(0, 1, parent_a), "777", Qt::DisplayRole);

  const auto parent_b = model_b->index(0, 0);
  EXPECT_TRUE(IsRed(model_b->index(0, 1, parent_b)));
}

//! SetActive(false) suspends highlighting; SetActive(true) re-enables it.

TEST_F(DiffHighlightControllerTest, SetActiveDisablesHighlighting)
{
  auto model_a = CreateModel();
  auto model_b = CreateModel();

  DiffHighlightController controller(model_a.get(), model_b.get());

  controller.SetActive(false);
  EXPECT_FALSE(controller.IsActive());

  model_a->setData(model_a->index(0, 1), "999", Qt::DisplayRole);
  EXPECT_TRUE(HasNoBackground(model_b->index(0, 1)));  // not highlighted

  controller.SetActive(true);
  model_a->setData(model_a->index(1, 1), "888", Qt::DisplayRole);
  EXPECT_TRUE(IsRed(model_b->index(1, 1)));  // highlighting resumed
}

}  // namespace sup::gui::test
