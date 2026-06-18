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

#include "sup/gui/component_objects/tree_highlight_controller.h"

#include <gtest/gtest.h>

#include <QColor>
#include <QStandardItem>
#include <QStandardItemModel>
#include <memory>

namespace sup::gui::test
{

class TreeHighlightControllerTest : public ::testing::Test
{
public:
  //! Builds a two-column model with one top-level branch holding a child, plus a second top-level
  //! row.
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

  static bool IsRed(const QModelIndex& index)
  {
    const auto background = index.data(Qt::BackgroundRole);
    return background.isValid() && (background.value<QColor>() == QColor(Qt::red));
  }

  static bool HasNoBackground(const QModelIndex& index)
  {
    return !index.data(Qt::BackgroundRole).isValid();
  }
};

TEST_F(TreeHighlightControllerTest, InitialState)
{
  const TreeHighlightController controller;
  EXPECT_TRUE(controller.IsActive());
}

//! Without a reference no highlight is produced when models differ.

TEST_F(TreeHighlightControllerTest, NoReference)
{
  auto model0 = CreateModel();
  auto model1 = CreateModel();

  TreeHighlightController controller;
  controller.AddViewModel(model0.get());
  controller.AddViewModel(model1.get());

  model0->setData(model0->index(1, 1), "differs", Qt::DisplayRole);
  EXPECT_TRUE(HasNoBackground(model1->index(1, 1)));
}

//! Setting the reference performs an initial highlight pass on all other models.

TEST_F(TreeHighlightControllerTest, SetReferenceHighlightsExistingDifferences)
{
  auto model0 = CreateModel();
  auto model1 = CreateModel();
  auto model2 = CreateModel();

  // model1 differs from the reference in a cell that exists before the reference is set
  model1->setData(model1->index(1, 1), "differs", Qt::DisplayRole);

  TreeHighlightController controller;
  controller.AddViewModel(model0.get());
  controller.AddViewModel(model1.get());
  controller.AddViewModel(model2.get());

  controller.SetReferenceViewModel(model0.get());

  // model1 differing cell is highlighted, model2 (equal) is not
  EXPECT_TRUE(IsRed(model1->index(1, 1)));
  EXPECT_TRUE(HasNoBackground(model2->index(1, 1)));
  // reference itself is never highlighted
  EXPECT_TRUE(HasNoBackground(model0->index(1, 1)));
}

//! A change in the reference is propagated to all non-reference models.

TEST_F(TreeHighlightControllerTest, ReferenceChangePropagatesToAllModels)
{
  auto model0 = CreateModel();
  auto model1 = CreateModel();
  auto model2 = CreateModel();

  TreeHighlightController controller;
  controller.AddViewModel(model0.get());
  controller.AddViewModel(model1.get());
  controller.AddViewModel(model2.get());
  controller.SetReferenceViewModel(model0.get());

  model0->setData(model0->index(0, 1), "999", Qt::DisplayRole);

  EXPECT_TRUE(IsRed(model1->index(0, 1)));
  EXPECT_TRUE(IsRed(model2->index(0, 1)));
}

//! A change in a non-reference model highlights only that model.

TEST_F(TreeHighlightControllerTest, NonReferenceChangeHighlightsOwnModel)
{
  auto model0 = CreateModel();
  auto model1 = CreateModel();
  auto model2 = CreateModel();

  TreeHighlightController controller;
  controller.AddViewModel(model0.get());
  controller.AddViewModel(model1.get());
  controller.AddViewModel(model2.get());
  controller.SetReferenceViewModel(model0.get());

  model1->setData(model1->index(0, 1), "999", Qt::DisplayRole);

  EXPECT_TRUE(IsRed(model1->index(0, 1)));
  EXPECT_TRUE(HasNoBackground(model2->index(0, 1)));
}

//! A model added after the reference is set is included in the highlighting.

TEST_F(TreeHighlightControllerTest, AddViewModelAfterReference)
{
  auto model0 = CreateModel();
  auto model1 = CreateModel();

  TreeHighlightController controller;
  controller.AddViewModel(model0.get());
  controller.SetReferenceViewModel(model0.get());

  model1->setData(model1->index(1, 1), "differs", Qt::DisplayRole);
  controller.AddViewModel(model1.get());

  // adding the model triggers an initial pass that highlights the existing difference
  EXPECT_TRUE(IsRed(model1->index(1, 1)));
}

//! Removing a model stops it from being highlighted.

TEST_F(TreeHighlightControllerTest, RemoveViewModel)
{
  auto model0 = CreateModel();
  auto model1 = CreateModel();
  auto model2 = CreateModel();

  TreeHighlightController controller;
  controller.AddViewModel(model0.get());
  controller.AddViewModel(model1.get());
  controller.AddViewModel(model2.get());
  controller.SetReferenceViewModel(model0.get());

  controller.RemoveViewModel(model1.get());

  model0->setData(model0->index(0, 1), "999", Qt::DisplayRole);

  EXPECT_TRUE(HasNoBackground(model1->index(0, 1)));  // removed, not highlighted
  EXPECT_TRUE(IsRed(model2->index(0, 1)));            // still highlighted
}

//! Choosing a different reference recreates the controllers; the new reference drives highlighting.

TEST_F(TreeHighlightControllerTest, ChangeReference)
{
  auto model0 = CreateModel();
  auto model1 = CreateModel();

  TreeHighlightController controller;
  controller.AddViewModel(model0.get());
  controller.AddViewModel(model1.get());
  controller.SetReferenceViewModel(model0.get());

  // switch reference to model1; now model0 is highlighted against model1
  controller.SetReferenceViewModel(model1.get());

  model1->setData(model1->index(0, 1), "999", Qt::DisplayRole);
  EXPECT_TRUE(IsRed(model0->index(0, 1)));
}

//! SetActive(false) suspends highlighting.

TEST_F(TreeHighlightControllerTest, SetActiveDisablesHighlighting)
{
  auto model0 = CreateModel();
  auto model1 = CreateModel();

  TreeHighlightController controller;
  controller.AddViewModel(model0.get());
  controller.AddViewModel(model1.get());
  controller.SetReferenceViewModel(model0.get());

  controller.SetActive(false);
  EXPECT_FALSE(controller.IsActive());

  model0->setData(model0->index(0, 1), "999", Qt::DisplayRole);
  EXPECT_TRUE(HasNoBackground(model1->index(0, 1)));
}

}  // namespace sup::gui::test
