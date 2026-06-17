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

#include <sup/gui/component_objects/tab_widget_controller.h>
#include <sup/gui/views/dtoeditor/configuration_tab_widget.h>
#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>

#include <gtest/gtest.h>

#include <QTabWidget>
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
};

TEST_F(ConfigurationPanelMirrorControllerTest, InitialState)
{
  ConfigurationPanelMirrorController controller;
  EXPECT_NE(controller.GetTabWidgetController(), nullptr);
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

}  // namespace sup::gui::test
