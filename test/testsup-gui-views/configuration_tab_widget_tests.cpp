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

#include "sup/gui/views/dtoeditor/configuration_tab_widget.h"

#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for ConfigurationTabWidget class.
 */
class ConfigurationTabWidgetTest : public ::testing::Test
{
public:
};

TEST_F(ConfigurationTabWidgetTest, InitialState)
{
  ConfigurationTabWidget widget;

  EXPECT_NE(widget.GetTabWidget(), nullptr);
  EXPECT_NE(widget.GetScrollArea(ConfigurationViewMode::kTree), nullptr);
  EXPECT_NE(widget.GetScrollArea(ConfigurationViewMode::kJSON), nullptr);
  EXPECT_NE(widget.GetMainTreeView(), nullptr);
}

}  // namespace sup::gui::test
