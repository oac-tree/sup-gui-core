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

#include "sup/gui/component_objects/tree_expand_controller.h"

#include <gtest/gtest.h>

#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <memory>

namespace sup::gui::test
{

/**
 * @brief Tests for TreeExpandController class.
 */
class TreeExpandControllerTest : public ::testing::Test
{
public:
  //! Builds a model with two top-level branches; the first child of the first branch has its own
  //! children (so we can test nested-branch translation).
  static std::unique_ptr<QStandardItemModel> CreateModel()
  {
    auto model = std::make_unique<QStandardItemModel>();
    for (int i = 0; i < 2; ++i)
    {
      auto* parent = new QStandardItem(QString("parent%1").arg(i));
      for (int j = 0; j < 2; ++j)
      {
        auto* child = new QStandardItem(QString("child%1_%2").arg(i).arg(j));
        if (i == 0 && j == 0)
        {
          child->appendRow(new QStandardItem("grandchild"));
        }
        parent->appendRow(child);
      }
      model->appendRow(parent);
    }
    return model;
  }
};

TEST_F(TreeExpandControllerTest, InitialState)
{
  const TreeExpandController controller;
  EXPECT_TRUE(controller.IsActive());
}

//! Expanding/collapsing a top-level branch in one tree mirrors to the other.

TEST_F(TreeExpandControllerTest, MirrorExpandCollapse)
{
  auto model1 = CreateModel();
  auto model2 = CreateModel();

  QTreeView tree1;
  QTreeView tree2;
  tree1.setModel(model1.get());
  tree2.setModel(model2.get());

  TreeExpandController controller;
  controller.AddTreeView(&tree1);
  controller.AddTreeView(&tree2);

  const auto branch1 = model1->index(0, 0);
  const auto branch2 = model2->index(0, 0);

  ASSERT_FALSE(tree2.isExpanded(branch2));

  tree1.expand(branch1);
  EXPECT_TRUE(tree2.isExpanded(branch2));

  tree1.collapse(branch1);
  EXPECT_FALSE(tree2.isExpanded(branch2));
}

//! A nested branch is correctly translated by its row path into the other tree's model.

TEST_F(TreeExpandControllerTest, MirrorNestedBranch)
{
  auto model1 = CreateModel();
  auto model2 = CreateModel();

  QTreeView tree1;
  QTreeView tree2;
  tree1.setModel(model1.get());
  tree2.setModel(model2.get());

  TreeExpandController controller;
  controller.AddTreeView(&tree1);
  controller.AddTreeView(&tree2);

  // first child of the first parent (path [0, 0]) is the branch with grandchildren
  const auto nested1 = model1->index(0, 0, model1->index(0, 0));
  const auto nested2 = model2->index(0, 0, model2->index(0, 0));
  ASSERT_TRUE(nested1.isValid());
  ASSERT_TRUE(nested2.isValid());

  tree1.expand(nested1);
  EXPECT_TRUE(tree2.isExpanded(nested2));
}

//! Mirroring is bidirectional.

TEST_F(TreeExpandControllerTest, MirrorIsBidirectional)
{
  auto model1 = CreateModel();
  auto model2 = CreateModel();

  QTreeView tree1;
  QTreeView tree2;
  tree1.setModel(model1.get());
  tree2.setModel(model2.get());

  TreeExpandController controller;
  controller.AddTreeView(&tree1);
  controller.AddTreeView(&tree2);

  tree2.expand(model2->index(1, 0));
  EXPECT_TRUE(tree1.isExpanded(model1->index(1, 0)));
}

//! SetActive(false) suspends mirroring; SetActive(true) re-enables it.

TEST_F(TreeExpandControllerTest, SetActiveDisablesMirroring)
{
  auto model1 = CreateModel();
  auto model2 = CreateModel();

  QTreeView tree1;
  QTreeView tree2;
  tree1.setModel(model1.get());
  tree2.setModel(model2.get());

  TreeExpandController controller;
  controller.AddTreeView(&tree1);
  controller.AddTreeView(&tree2);

  controller.SetActive(false);
  EXPECT_FALSE(controller.IsActive());

  tree1.expand(model1->index(0, 0));
  EXPECT_FALSE(tree2.isExpanded(model2->index(0, 0)));  // not mirrored

  controller.SetActive(true);
  tree1.expand(model1->index(1, 0));
  EXPECT_TRUE(tree2.isExpanded(model2->index(1, 0)));  // mirroring resumed
}

//! RemoveTreeView stops mirroring to the removed view.

TEST_F(TreeExpandControllerTest, RemoveTreeView)
{
  auto model1 = CreateModel();
  auto model2 = CreateModel();

  QTreeView tree1;
  QTreeView tree2;
  tree1.setModel(model1.get());
  tree2.setModel(model2.get());

  TreeExpandController controller;
  controller.AddTreeView(&tree1);
  controller.AddTreeView(&tree2);

  controller.RemoveTreeView(&tree2);

  tree1.expand(model1->index(0, 0));
  EXPECT_FALSE(tree2.isExpanded(model2->index(0, 0)));  // no longer mirrored
}

//! Destroying a registered view removes it automatically; no crash on subsequent expand.

TEST_F(TreeExpandControllerTest, DestroyedViewIsRemovedAutomatically)
{
  auto model1 = CreateModel();
  auto model2 = CreateModel();
  auto model3 = CreateModel();

  QTreeView tree1;
  QTreeView tree2;
  tree1.setModel(model1.get());
  tree2.setModel(model2.get());

  TreeExpandController controller;
  controller.AddTreeView(&tree1);
  controller.AddTreeView(&tree2);

  {
    QTreeView tree3;
    tree3.setModel(model3.get());
    controller.AddTreeView(&tree3);
  }  // tree3 destroyed here

  // expanding must not crash despite tree3 being gone
  tree1.expand(model1->index(0, 0));
  EXPECT_TRUE(tree2.isExpanded(model2->index(0, 0)));
}

}  // namespace sup::gui::test
