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

#include "sup/gui/views/dtoeditor/configuration_panel_mirror_controller.h"

#include <sup/gui/component_objects/scroll_area_mirror_controller.h>
#include <sup/gui/component_objects/tab_widget_controller.h>
#include <sup/gui/component_objects/tree_expand_controller.h>
#include <sup/gui/views/dtoeditor/configuration_tab_widget.h>
#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>

#include <gtest/gtest.h>

#include <QAbstractItemView>
#include <QAbstractScrollArea>
#include <QApplication>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QTabWidget>
#include <QTreeView>
#include <memory>

namespace sup::gui::test
{

/**
 * @brief Tests for ConfigurationPanelMirrorController class.
 */
class ConfigurationPanelMirrorControllerTest : public ::testing::Test
{
public:
  /**
   * @brief Creates a panel containing a ConfigurationTabWidget as its main editor.
   */
  static std::unique_ptr<SplittableComboPanel> CreatePanel()
  {
    auto editor = std::make_unique<ConfigurationTabWidget>();
    auto item_list_callback = []() { return std::vector<mvvm::SessionItem*>(); };
    return std::make_unique<SplittableComboPanel>(std::move(editor), item_list_callback);
  }

  /**
   * @brief Returns the tab widget of the panel's main editor.
   */
  static QTabWidget* GetTabWidget(SplittableComboPanel* panel)
  {
    return panel->GetMainEditor<ConfigurationTabWidget>()->GetTabWidget();
  }

  /**
   * @brief Returns the JSON view scroll area of the panel's main editor.
   */
  static QAbstractScrollArea* GetJsonArea(SplittableComboPanel* panel)
  {
    return panel->GetMainEditor<ConfigurationTabWidget>()->GetScrollArea(
        ConfigurationViewMode::kJSON);
  }

  /**
   * @brief Makes the JSON view current, fills it with enough text and shows it, so that its scroll
   * area gets a real (non-zero) scrollbar range.
   */
  static void PrepareJsonView(SplittableComboPanel* panel)
  {
    panel->GetMainEditor<ConfigurationTabWidget>()->GetTabWidget()->setCurrentIndex(1);

    if (auto* edit = qobject_cast<QPlainTextEdit*>(GetJsonArea(panel)); edit != nullptr)
    {
      QString text;
      for (int i = 0; i < 200; ++i)
      {
        text += QString("line %1\n").arg(i);
      }
      edit->setPlainText(text);
    }

    panel->resize(200, 200);
    panel->show();
  }

  /**
   * @brief Returns the tree view scroll area of the panel's main editor.
   */
  static QAbstractScrollArea* GetTreeArea(SplittableComboPanel* panel)
  {
    return panel->GetMainEditor<ConfigurationTabWidget>()->GetScrollArea(
        ConfigurationViewMode::kTree);
  }

  /**
   * @brief Assigns a model with enough rows to the tree view and shows it, so that its scroll area
   * gets a real (non-zero) scrollbar range.
   */
  static void PrepareTreeView(SplittableComboPanel* panel, QAbstractItemModel* model)
  {
    if (auto* view = qobject_cast<QAbstractItemView*>(GetTreeArea(panel)); view != nullptr)
    {
      view->setModel(model);
      // ScrollPerPixel derives the scrollbar range from the total content height, which is
      // available without the view being painted (unlike the default ScrollPerItem mode).
      view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
      view->doItemsLayout();
    }

    panel->resize(200, 200);
    panel->show();

    // flush layout so the tree view establishes its viewport geometry and scrollbar range
    QApplication::processEvents();
  }

  /**
   * @brief Returns the main tree view of the panel's editor.
   */
  static QTreeView* GetMainTreeView(SplittableComboPanel* panel)
  {
    return panel->GetMainEditor<ConfigurationTabWidget>()->GetMainTreeView();
  }

  /**
   * @brief Builds a model with a single top-level branch holding two children.
   */
  static std::unique_ptr<QStandardItemModel> CreateBranchModel()
  {
    auto model = std::make_unique<QStandardItemModel>();
    auto* parent = new QStandardItem("parent");
    parent->appendRow(new QStandardItem("child0"));
    parent->appendRow(new QStandardItem("child1"));
    model->appendRow(parent);
    return model;
  }
};

TEST_F(ConfigurationPanelMirrorControllerTest, InitialState)
{
  ConfigurationPanelMirrorController controller;
  EXPECT_NE(controller.GetTabWidgetController(), nullptr);
  EXPECT_NE(controller.GetJsonAreaController(), nullptr);
  EXPECT_NE(controller.GetTreeAreaController(), nullptr);
  EXPECT_NE(controller.GetTreeExpandController(), nullptr);
}

TEST_F(ConfigurationPanelMirrorControllerTest, ProcessNullPanel)
{
  ConfigurationPanelMirrorController controller;

  // processing a null panel should not crash
  EXPECT_NO_THROW(controller.ProcessPanelAdd(nullptr));
  EXPECT_NO_THROW(controller.ProcessPanelRemove(nullptr));
}

//! Adding two panels should synchronise their tab selection.

TEST_F(ConfigurationPanelMirrorControllerTest, ProcessPanelAddMirrorsTabSelection)
{
  ConfigurationPanelMirrorController controller;

  auto panel1 = CreatePanel();
  auto panel2 = CreatePanel();

  controller.ProcessPanelAdd(panel1.get());
  controller.ProcessPanelAdd(panel2.get());

  auto* tab1 = GetTabWidget(panel1.get());
  auto* tab2 = GetTabWidget(panel2.get());

  // both editors start on the first tab
  ASSERT_EQ(tab1->currentIndex(), 0);
  ASSERT_EQ(tab2->currentIndex(), 0);

  // switching tab in the first panel should mirror to the second
  tab1->setCurrentIndex(1);
  EXPECT_EQ(tab2->currentIndex(), 1);

  // and the other way around
  tab2->setCurrentIndex(0);
  EXPECT_EQ(tab1->currentIndex(), 0);
}

//! After removing a panel, its tab selection should no longer be mirrored.

TEST_F(ConfigurationPanelMirrorControllerTest, ProcessPanelRemoveStopsMirroring)
{
  ConfigurationPanelMirrorController controller;

  auto panel1 = CreatePanel();
  auto panel2 = CreatePanel();

  controller.ProcessPanelAdd(panel1.get());
  controller.ProcessPanelAdd(panel2.get());

  controller.ProcessPanelRemove(panel2.get());

  auto* tab1 = GetTabWidget(panel1.get());
  auto* tab2 = GetTabWidget(panel2.get());

  // switching tab in the first panel should no longer affect the removed one
  tab1->setCurrentIndex(1);
  EXPECT_EQ(tab2->currentIndex(), 0);
}

//! Adding two panels should couple the scroll positions of their JSON views.

TEST_F(ConfigurationPanelMirrorControllerTest, ProcessPanelAddMirrorsJsonScroll)
{
  ConfigurationPanelMirrorController controller;

  auto panel1 = CreatePanel();
  auto panel2 = CreatePanel();

  controller.ProcessPanelAdd(panel1.get());
  controller.ProcessPanelAdd(panel2.get());

  PrepareJsonView(panel1.get());
  PrepareJsonView(panel2.get());

  auto* area1 = GetJsonArea(panel1.get());
  auto* area2 = GetJsonArea(panel2.get());

  // make sure the JSON view has a real scroll range
  ASSERT_GT(area1->verticalScrollBar()->maximum(), 5);

  // scrolling the first JSON view should mirror to the second
  area1->verticalScrollBar()->setValue(5);
  EXPECT_EQ(area2->verticalScrollBar()->value(), 5);
}

//! After removing a panel, its JSON view scroll position should no longer be mirrored.

TEST_F(ConfigurationPanelMirrorControllerTest, ProcessPanelRemoveStopsJsonScroll)
{
  ConfigurationPanelMirrorController controller;

  auto panel1 = CreatePanel();
  auto panel2 = CreatePanel();

  controller.ProcessPanelAdd(panel1.get());
  controller.ProcessPanelAdd(panel2.get());

  controller.ProcessPanelRemove(panel2.get());

  PrepareJsonView(panel1.get());
  PrepareJsonView(panel2.get());

  auto* area1 = GetJsonArea(panel1.get());
  auto* area2 = GetJsonArea(panel2.get());

  ASSERT_GT(area1->verticalScrollBar()->maximum(), 5);

  // scrolling the first JSON view should no longer affect the removed one
  area1->verticalScrollBar()->setValue(5);
  EXPECT_EQ(area2->verticalScrollBar()->value(), 0);
}

//! Adding two panels should couple the scroll positions of their tree views.

TEST_F(ConfigurationPanelMirrorControllerTest, ProcessPanelAddMirrorsTreeScroll)
{
  QStringList rows;
  for (int i = 0; i < 200; ++i)
  {
    rows << QString("row %1").arg(i);
  }
  QStringListModel model(rows);

  ConfigurationPanelMirrorController controller;

  auto panel1 = CreatePanel();
  auto panel2 = CreatePanel();

  controller.ProcessPanelAdd(panel1.get());
  controller.ProcessPanelAdd(panel2.get());

  PrepareTreeView(panel1.get(), &model);
  PrepareTreeView(panel2.get(), &model);

  auto* area1 = GetTreeArea(panel1.get());
  auto* area2 = GetTreeArea(panel2.get());

  // make sure the tree view has a real scroll range
  ASSERT_GT(area1->verticalScrollBar()->maximum(), 5);

  // scrolling the first tree view should mirror to the second
  area1->verticalScrollBar()->setValue(5);
  EXPECT_EQ(area2->verticalScrollBar()->value(), 5);
}

//! After removing a panel, its tree view scroll position should no longer be mirrored.

TEST_F(ConfigurationPanelMirrorControllerTest, ProcessPanelRemoveStopsTreeScroll)
{
  QStringList rows;
  for (int i = 0; i < 200; ++i)
  {
    rows << QString("row %1").arg(i);
  }
  QStringListModel model(rows);

  ConfigurationPanelMirrorController controller;

  auto panel1 = CreatePanel();
  auto panel2 = CreatePanel();

  controller.ProcessPanelAdd(panel1.get());
  controller.ProcessPanelAdd(panel2.get());

  controller.ProcessPanelRemove(panel2.get());

  PrepareTreeView(panel1.get(), &model);
  PrepareTreeView(panel2.get(), &model);

  auto* area1 = GetTreeArea(panel1.get());
  auto* area2 = GetTreeArea(panel2.get());

  ASSERT_GT(area1->verticalScrollBar()->maximum(), 5);

  // scrolling the first tree view should no longer affect the removed one
  area1->verticalScrollBar()->setValue(5);
  EXPECT_EQ(area2->verticalScrollBar()->value(), 0);
}

//! Adding two panels should couple the expand/collapse state of their tree views.

TEST_F(ConfigurationPanelMirrorControllerTest, ProcessPanelAddMirrorsTreeExpand)
{
  ConfigurationPanelMirrorController controller;

  auto panel1 = CreatePanel();
  auto panel2 = CreatePanel();

  controller.ProcessPanelAdd(panel1.get());
  controller.ProcessPanelAdd(panel2.get());

  auto model1 = CreateBranchModel();
  auto model2 = CreateBranchModel();
  auto* tree1 = GetMainTreeView(panel1.get());
  auto* tree2 = GetMainTreeView(panel2.get());
  tree1->setModel(model1.get());
  tree2->setModel(model2.get());

  const auto branch1 = model1->index(0, 0);
  const auto branch2 = model2->index(0, 0);
  ASSERT_FALSE(tree2->isExpanded(branch2));

  // expanding the branch in the first tree should mirror to the second
  tree1->expand(branch1);
  EXPECT_TRUE(tree2->isExpanded(branch2));

  // and collapsing too
  tree1->collapse(branch1);
  EXPECT_FALSE(tree2->isExpanded(branch2));
}

//! After removing a panel, its tree view expand/collapse state should no longer be mirrored.

TEST_F(ConfigurationPanelMirrorControllerTest, ProcessPanelRemoveStopsTreeExpand)
{
  ConfigurationPanelMirrorController controller;

  auto panel1 = CreatePanel();
  auto panel2 = CreatePanel();

  controller.ProcessPanelAdd(panel1.get());
  controller.ProcessPanelAdd(panel2.get());

  controller.ProcessPanelRemove(panel2.get());

  auto model1 = CreateBranchModel();
  auto model2 = CreateBranchModel();
  auto* tree1 = GetMainTreeView(panel1.get());
  auto* tree2 = GetMainTreeView(panel2.get());
  tree1->setModel(model1.get());
  tree2->setModel(model2.get());

  // expanding the branch in the first tree should no longer affect the removed one
  tree1->expand(model1->index(0, 0));
  EXPECT_FALSE(tree2->isExpanded(model2->index(0, 0)));
}

}  // namespace sup::gui::test
