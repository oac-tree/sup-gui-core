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

#include "sup/gui/views/dtoeditor/configuration_splittable_widget.h"

#include <sup/gui/component_objects/tree_highlight_controller.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/views/dtoeditor/configuration_panel_mirror_controller.h>
#include <sup/gui/views/dtoeditor/configuration_tab_widget.h>
#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>
#include <sup/gui/views/dtoeditor/splittable_editor_controller.h>

#include <mvvm/model/item_selection.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>
#include <testutils/test_settings_manager.h>

#include <QSignalSpy>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTreeView>
#include <memory>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(mvvm::ItemSelection)
#endif

namespace sup::gui::test
{

/**
 * @brief Tests for ConfigurationSplittableWidget class.
 */
class ConfigurationSplittableWidgetTest : public ::testing::Test
{
public:
  TestSettingsManager m_settings_manager;
};

TEST_F(ConfigurationSplittableWidgetTest, InitialState)
{
  ConfigurationSplittableWidget editor(m_settings_manager);
  editor.show();

  ASSERT_NE(editor.GetController(), nullptr);
  ASSERT_NE(editor.GetPanelMirrorController(), nullptr);

  auto splitter = editor.findChild<QSplitter*>();

  // empty splitter
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 0);
}

TEST_F(ConfigurationSplittableWidgetTest, CreatePanel)
{
  ConfigurationSplittableWidget editor(m_settings_manager);
  editor.show();

  auto splitter = editor.findChild<QSplitter*>();

  // add first panel
  editor.CreatePanel();

  ASSERT_EQ(splitter->count(), 1);
}

TEST_F(ConfigurationSplittableWidgetTest, SetModelAfterPanelCreation)
{
  ConfigurationModel model;

  ConfigurationSplittableWidget editor(m_settings_manager);
  editor.show();

  editor.CreatePanel();
  editor.SetModel(&model);
  editor.CreatePanel();

  auto panels = editor.GetController()->GetPanels();
  ASSERT_EQ(panels.size(), 2);
  EXPECT_EQ(panels.at(0)->GetModel(), &model);
  EXPECT_EQ(panels.at(1)->GetModel(), &model);
}

TEST_F(ConfigurationSplittableWidgetTest, FocusPanelSelectionChanged)
{
  const ConfigurationItem item;
  const mvvm::ItemSelection selection(&item);

  ConfigurationSplittableWidget editor(m_settings_manager);
  editor.show();

  editor.CreatePanel();

  QSignalSpy spy_selection_changed(&editor,
                                   &ConfigurationSplittableWidget::focusPanelSelectionChanged);

  // emit selection changed from controller
  editor.GetController()->focusPanelSelectionChanged(selection);
  ASSERT_EQ(spy_selection_changed.count(), 1);
  EXPECT_EQ(mvvm::test::TakeValue<mvvm::ItemSelection>(spy_selection_changed), selection);
}

//! A reference panel change in the splitter controller propagates to the highlight controller.

TEST_F(ConfigurationSplittableWidgetTest, ReferencePanelPropagatesToHighlightController)
{
  ConfigurationSplittableWidget editor(m_settings_manager);
  editor.show();

  editor.CreatePanel();
  editor.CreatePanel();

  auto panels = editor.GetController()->GetPanels();
  ASSERT_EQ(panels.size(), 2);

  // give the first panel's tree view a known model
  QStandardItemModel model;
  panels.at(0)->GetMainEditor<ConfigurationTabWidget>()->GetMainTreeView()->setModel(&model);

  auto* highlight_controller = editor.GetPanelMirrorController()->GetTreeHighlightController();

  // the splitter controller marking the first panel as reference must reach the highlight
  // controller
  editor.GetController()->referencePanelChanged(panels.at(0));
  EXPECT_EQ(highlight_controller->GetReferenceViewModel(), &model);

  // clearing the reference propagates as well
  editor.GetController()->referencePanelChanged(nullptr);
  EXPECT_EQ(highlight_controller->GetReferenceViewModel(), nullptr);
}

}  // namespace sup::gui::test