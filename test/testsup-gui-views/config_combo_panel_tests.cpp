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

#include <mvvm/model/item_selection.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QSignalSpy>
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

  // toolbar is initialised, but doesn't have meaningfull state. The state will appear only after
  // the model set.
  EXPECT_TRUE(toolbar->GetCurrentText().isEmpty());
  EXPECT_EQ(toolbar->GetCurrentIndex(), -1);

  widget.SetConfiguration(nullptr);
  EXPECT_EQ(toolbar->GetCurrentText(), QString(ConfigComboAdapter::kNoSelection));
  EXPECT_EQ(toolbar->GetCurrentIndex(), 0);
}

TEST_F(ConfigComboPanelTest, SetEmptyModel)
{
  ConfigComboPanel widget;
  widget.show();

  const QSignalSpy selected_configuration_changed_spy(
      &widget, &ConfigComboPanel::selectedConfigurationChanged);

  ConfigurationModel model;
  widget.SetModel(&model);

  EXPECT_EQ(selected_configuration_changed_spy.count(), 0);

  EXPECT_EQ(widget.GetModel(), &model);
  EXPECT_EQ(widget.GetCurrentConfiguration(), nullptr);

  auto stacked_widget = widget.findChild<QStackedWidget*>();
  ASSERT_NE(stacked_widget, nullptr);
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(ConfigComboPanel::PlaceholderMode::kShowPlaceholder));

  auto toolbar = widget.findChild<ComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);

  EXPECT_EQ(toolbar->GetCurrentText(), ConfigComboAdapter::kNoSelection);
  EXPECT_EQ(toolbar->GetCurrentIndex(), 0);
}

TEST_F(ConfigComboPanelTest, SetConfigurationProgrammatically)
{
  ConfigurationModel model;
  auto configuration0 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration0->SetDisplayName("Config0");
  auto configuration1 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration1->SetDisplayName("Config1");

  ConfigComboPanel widget;
  widget.show();

  QSignalSpy selected_changed_spy(&widget, &ConfigComboPanel::selectedConfigurationChanged);

  widget.SetModel(&model);

  EXPECT_EQ(widget.GetModel(), &model);
  EXPECT_EQ(widget.GetCurrentConfiguration(), nullptr);

  auto stacked_widget = widget.findChild<QStackedWidget*>();
  ASSERT_NE(stacked_widget, nullptr);
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(ConfigComboPanel::PlaceholderMode::kShowPlaceholder));

  auto toolbar = widget.findChild<ComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);

  EXPECT_EQ(toolbar->GetCurrentText(), ConfigComboAdapter::kNoSelection);
  EXPECT_EQ(toolbar->GetCurrentIndex(), 0);

  widget.SetConfiguration(configuration0);

  ASSERT_EQ(selected_changed_spy.count(), 1);
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::ItemSelection>(selected_changed_spy),
            mvvm::ItemSelection(configuration0));

  EXPECT_EQ(widget.GetCurrentConfiguration(), configuration0);
  EXPECT_EQ(toolbar->GetCurrentText(), QString::fromStdString(configuration0->GetDisplayName()));
  EXPECT_EQ(toolbar->GetCurrentIndex(), 1);

  // check that the stacked widget now is showing the editor, not the placeholder
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(ConfigComboPanel::PlaceholderMode::kShowEditor));

  // setting same configuration again doesn't emit signal
  widget.SetConfiguration(configuration0);
  EXPECT_EQ(selected_changed_spy.count(), 0);

  // setting null configuration
  widget.SetConfiguration(nullptr);
  ASSERT_EQ(selected_changed_spy.count(), 1);
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::ItemSelection>(selected_changed_spy),
            mvvm::ItemSelection(nullptr));
  EXPECT_EQ(widget.GetCurrentConfiguration(), nullptr);
  EXPECT_EQ(toolbar->GetCurrentText(), ConfigComboAdapter::kNoSelection);
  EXPECT_EQ(toolbar->GetCurrentIndex(), 0);

  // placeholder widget is back again
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(ConfigComboPanel::PlaceholderMode::kShowPlaceholder));
}

}  // namespace sup::gui::test
