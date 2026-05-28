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

#include "sup/gui/views/dtoeditor/config_combo_panel.h"

#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/views/dtoeditor/config_combo_adapter.h>
#include <sup/gui/views/dtoeditor/config_tab_widget.h>
#include <sup/gui/widgets/combo_tool_bar.h>

#include <gtest/gtest.h>

#include <QStackedWidget>

namespace sup::gui::test
{

/**
 * @brief Tests for ConfigComboPanel class.
 */
class ConfigComboPanelTest : public ::testing::Test
{
};

TEST_F(ConfigComboPanelTest, InitialState)
{
  ConfigComboPanel widget;
  widget.show();

  EXPECT_EQ(widget.GetModel(), nullptr);
  EXPECT_EQ(widget.GetCurrentConfiguration(), nullptr);

  auto stacked_widget = widget.findChild<QStackedWidget*>();
  ASSERT_NE(stacked_widget, nullptr);
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(ConfigComboPanel::PlaceholderMode::kShowPlaceholder));

  auto toolbar = widget.findChild<ComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);

  // toolbar is initialised, but doesn't have meaningfull state
  // Shouldn't we set it to "no selection" already?
  EXPECT_TRUE(toolbar->GetCurrentText().isEmpty());
  EXPECT_EQ(toolbar->GetCurrentIndex(), -1);

  widget.SetConfiguration(nullptr);
  EXPECT_EQ(toolbar->GetCurrentText(), QString(ConfigComboAdapter::kNoSelection));
  EXPECT_EQ(toolbar->GetCurrentIndex(), 0);
}


}  // namespace sup::gui::test
