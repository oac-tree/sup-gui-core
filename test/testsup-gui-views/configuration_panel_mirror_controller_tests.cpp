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

#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for ConfigurationPanelMirrorController class.
 */
class ConfigurationPanelMirrorControllerTest : public ::testing::Test
{
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

}  // namespace sup::gui::test
