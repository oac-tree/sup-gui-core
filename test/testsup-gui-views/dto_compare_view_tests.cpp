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

#include "sup/gui/views/dtoeditor/dto_compare_view.h"

#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/views/dtoeditor/dto_explorer_panel.h>
#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>
#include <sup/gui/views/dtoeditor/splittable_editor_controller.h>
#include <sup/gui/views/dtoeditor/splittable_editor_widget.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

namespace sup::gui::test
{

/**
 * @brief Tests for DtoCompareViewTest class.
 */
class DtoCompareViewTest : public ::testing::Test
{
};

// FIXME me: This test is currently disabled, because of the "features" of ItemComponentProvider
// selection mechanism.
TEST_F(DtoCompareViewTest, DISABLED_SelectionLogic)
{
  ConfigurationModel model;
  auto configuration0 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration0->SetDisplayName("Config0");
  auto configuration1 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration1->SetDisplayName("Config1");

  DtoCompareView view;
  view.show();

  auto explorer_panel = view.findChild<DtoExplorerPanel*>();
  ASSERT_NE(explorer_panel, nullptr);
  auto splittable_config_editor = view.findChild<SplittableEditorWidget*>();
  auto controller = splittable_config_editor->GetController();

  view.SetConfigurationModel(&model);

  auto panels = controller->GetPanels();
  ASSERT_EQ(panels.size(), 1);

  // no selection
  EXPECT_EQ(panels.at(0)->GetCurrentItem(), nullptr);
  EXPECT_EQ(panels.at(0)->GetModel(), &model);
  EXPECT_EQ(explorer_panel->GetSelection(), mvvm::ItemSelection());

  QSignalSpy explorer_selection_changed_spy(explorer_panel, &DtoExplorerPanel::SelectionChanged);
  QSignalSpy editor_selection_changed_spy(controller,
                                          &SplittableEditorController::focusPanelSelectionChanged);
  QSignalSpy controller_selection_changed_spy(
      controller, &SplittableEditorController::focusPanelSelectionChanged);

  // making selection via explorer
  explorer_panel->SetSelection(mvvm::ItemSelection(configuration0));

  ASSERT_EQ(explorer_selection_changed_spy.count(), 1);
  ASSERT_EQ(editor_selection_changed_spy.count(), 1);
  ASSERT_EQ(controller_selection_changed_spy.count(), 1);

  EXPECT_EQ(panels.at(0)->GetCurrentItem(), configuration0);
  EXPECT_EQ(explorer_panel->GetSelection(), mvvm::ItemSelection(configuration0));

  // making selection via panel
  panels.at(0)->SetItem(configuration1);

  ASSERT_EQ(explorer_selection_changed_spy.count(), 3);  // FIXME, should be 2
  ASSERT_EQ(editor_selection_changed_spy.count(), 4);    // FIXME, should be 2
  ASSERT_EQ(controller_selection_changed_spy.count(), 4);
  // The problem here is that ItemComponentProvider selection mechanism has "features".
  // When we set the new item, it first clears the selection, and then sets the right item, which
  // causes 2 SelectionChanged signals instead of 1. Fix test after ItemViewComponentProvider
  // refactoring.

  EXPECT_EQ(panels.at(0)->GetCurrentItem(), configuration1);
  EXPECT_EQ(explorer_panel->GetSelection(), mvvm::ItemSelection(configuration1));
}
}  // namespace sup::gui::test
