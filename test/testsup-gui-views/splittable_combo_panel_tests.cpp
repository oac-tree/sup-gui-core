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

#include "sup/gui/views/dtoeditor/splittable_combo_panel.h"

#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/views/dtoeditor/config_combo_adapter.h>
#include <sup/gui/views/dtoeditor/config_tab_widget.h>
#include <sup/gui/widgets/combo_tool_bar.h>

#include <mvvm/model/item_selection.h>
#include <mvvm/signals/model_listener.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QMenu>
#include <QSignalSpy>
#include <QStackedWidget>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(mvvm::ItemSelection)
#endif

namespace sup::gui::test
{

/**
 * @brief Tests for SplittableComboPanel class.
 */
class SplittableComboPanelTest : public ::testing::Test
{
};

TEST_F(SplittableComboPanelTest, InitialState)
{
  SplittableComboPanel widget;
  widget.show();

  EXPECT_EQ(widget.GetModel(), nullptr);
  EXPECT_EQ(widget.GetCurrentItem(), nullptr);

  auto stacked_widget = widget.findChild<QStackedWidget*>();
  ASSERT_NE(stacked_widget, nullptr);
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(SplittableComboPanel::PlaceholderMode::kShowPlaceholder));

  auto toolbar = widget.findChild<ComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);

  // toolbar is initialised, but doesn't have meaningfull state. The state will appear only after
  // the model set.
  EXPECT_TRUE(toolbar->GetCurrentText().isEmpty());
  EXPECT_EQ(toolbar->GetCurrentIndex(), -1);

  widget.SetItem(nullptr);
  EXPECT_EQ(toolbar->GetCurrentText(), QString(ConfigComboAdapter::kNoSelection));
  EXPECT_EQ(toolbar->GetCurrentIndex(), 0);
}

TEST_F(SplittableComboPanelTest, SetEmptyModel)
{
  SplittableComboPanel widget;
  widget.show();

  const QSignalSpy selected_configuration_changed_spy(
      &widget, &SplittableComboPanel::selectedConfigurationChanged);

  ConfigurationModel model;
  widget.SetModel(&model);

  EXPECT_EQ(selected_configuration_changed_spy.count(), 0);

  EXPECT_EQ(widget.GetModel(), &model);
  EXPECT_EQ(widget.GetCurrentItem(), nullptr);

  auto stacked_widget = widget.findChild<QStackedWidget*>();
  ASSERT_NE(stacked_widget, nullptr);
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(SplittableComboPanel::PlaceholderMode::kShowPlaceholder));

  auto toolbar = widget.findChild<ComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);

  EXPECT_EQ(toolbar->GetCurrentText(), ConfigComboAdapter::kNoSelection);
  EXPECT_EQ(toolbar->GetCurrentIndex(), 0);
}

TEST_F(SplittableComboPanelTest, SetConfigurationProgrammatically)
{
  ConfigurationModel model;
  auto configuration0 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration0->SetDisplayName("Config0");
  auto configuration1 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration1->SetDisplayName("Config1");

  SplittableComboPanel widget;
  widget.show();

  QSignalSpy selected_changed_spy(&widget, &SplittableComboPanel::selectedConfigurationChanged);

  widget.SetModel(&model);

  EXPECT_EQ(widget.GetModel(), &model);
  EXPECT_EQ(widget.GetCurrentItem(), nullptr);

  auto stacked_widget = widget.findChild<QStackedWidget*>();
  ASSERT_NE(stacked_widget, nullptr);
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(SplittableComboPanel::PlaceholderMode::kShowPlaceholder));

  auto toolbar = widget.findChild<ComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);

  EXPECT_EQ(toolbar->GetCurrentText(), ConfigComboAdapter::kNoSelection);
  EXPECT_EQ(toolbar->GetCurrentIndex(), 0);

  widget.SetItem(configuration0);

  ASSERT_EQ(selected_changed_spy.count(), 1);
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::ItemSelection>(selected_changed_spy),
            mvvm::ItemSelection(configuration0));

  EXPECT_EQ(widget.GetCurrentItem(), configuration0);
  EXPECT_EQ(toolbar->GetCurrentText(), QString::fromStdString(configuration0->GetDisplayName()));
  EXPECT_EQ(toolbar->GetCurrentIndex(), 1);

  // check that the stacked widget now is showing the editor, not the placeholder
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(SplittableComboPanel::PlaceholderMode::kShowEditor));

  // setting same configuration again doesn't emit signal
  widget.SetItem(configuration0);
  EXPECT_EQ(selected_changed_spy.count(), 0);

  // setting null configuration
  widget.SetItem(nullptr);
  ASSERT_EQ(selected_changed_spy.count(), 1);
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::ItemSelection>(selected_changed_spy),
            mvvm::ItemSelection(nullptr));
  EXPECT_EQ(widget.GetCurrentItem(), nullptr);
  EXPECT_EQ(toolbar->GetCurrentText(), ConfigComboAdapter::kNoSelection);
  EXPECT_EQ(toolbar->GetCurrentIndex(), 0);

  // placeholder widget is back again
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(SplittableComboPanel::PlaceholderMode::kShowPlaceholder));
}

TEST_F(SplittableComboPanelTest, SetConfigurationViaToolbar)
{
  ConfigurationModel model;
  auto configuration0 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration0->SetDisplayName("Config0");
  auto configuration1 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration1->SetDisplayName("Config1");

  SplittableComboPanel widget;
  widget.show();

  QSignalSpy selected_changed_spy(&widget, &SplittableComboPanel::selectedConfigurationChanged);

  widget.SetModel(&model);

  EXPECT_EQ(widget.GetModel(), &model);
  EXPECT_EQ(widget.GetCurrentItem(), nullptr);

  auto stacked_widget = widget.findChild<QStackedWidget*>();
  ASSERT_NE(stacked_widget, nullptr);
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(SplittableComboPanel::PlaceholderMode::kShowPlaceholder));

  auto toolbar = widget.findChild<ComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);

  EXPECT_EQ(toolbar->GetCurrentText(), ConfigComboAdapter::kNoSelection);
  EXPECT_EQ(toolbar->GetCurrentIndex(), 0);

  // select first configuration via toolbar
  toolbar->SetCurrentIndex(1);

  ASSERT_EQ(selected_changed_spy.count(), 1);
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::ItemSelection>(selected_changed_spy),
            mvvm::ItemSelection(configuration0));

  EXPECT_EQ(widget.GetCurrentItem(), configuration0);
  EXPECT_EQ(toolbar->GetCurrentText(), QString::fromStdString(configuration0->GetDisplayName()));
  EXPECT_EQ(toolbar->GetCurrentIndex(), 1);

  // select second configuration via toolbar menu
  // make menu pop-up to populate it with actions
  toolbar->GetMenu()->popup(toolbar->mapToGlobal(toolbar->rect().bottomLeft()));
  auto actions = toolbar->GetMenu()->actions();
  ASSERT_EQ(actions.size(), 3);  // no selection + 2 configurations

  // trigger action for second configuration
  actions.at(2)->trigger();

  ASSERT_EQ(selected_changed_spy.count(), 1);
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::ItemSelection>(selected_changed_spy),
            mvvm::ItemSelection(configuration1));

  EXPECT_EQ(widget.GetCurrentItem(), configuration1);
  EXPECT_EQ(toolbar->GetCurrentText(), QString::fromStdString(configuration1->GetDisplayName()));
  EXPECT_EQ(toolbar->GetCurrentIndex(), 2);
}

TEST_F(SplittableComboPanelTest, SplitViewCloseViewRequestsArePropagated)
{
  SplittableComboPanel widget;
  widget.show();

  auto toolbar = widget.findChild<ComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);

  const QSignalSpy spy_split(&widget, &SplittableComboPanel::splitViewRequest);
  const QSignalSpy spy_close(&widget, &SplittableComboPanel::closeViewRequest);

  // trigger split view request
  toolbar->splitViewRequest();
  ASSERT_EQ(spy_split.count(), 1);

  // trigger close view request
  toolbar->closeViewRequest();
  ASSERT_EQ(spy_close.count(), 1);
}

TEST_F(SplittableComboPanelTest, ChangeToPlaceholderWidgetOnConfigDeletion)
{
  ConfigurationModel model;
  auto configuration0 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration0->SetDisplayName("Config0");

  SplittableComboPanel widget;
  widget.show();

  const QSignalSpy selected_changed_spy(&widget,
                                        &SplittableComboPanel::selectedConfigurationChanged);
  widget.SetModel(&model);

  // select the configuration
  widget.SetItem(configuration0);
  ASSERT_EQ(selected_changed_spy.count(), 1);

  auto stacked_widget = widget.findChild<QStackedWidget*>();
  ASSERT_NE(stacked_widget, nullptr);
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(SplittableComboPanel::PlaceholderMode::kShowEditor));

  auto toolbar = widget.findChild<ComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);
  EXPECT_EQ(toolbar->GetCurrentText(), QString::fromStdString(configuration0->GetDisplayName()));
  EXPECT_EQ(toolbar->GetCurrentIndex(), 1);

  // delete item
  model.RemoveItem(configuration0);

  // expecting placeholder widget
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(SplittableComboPanel::PlaceholderMode::kShowPlaceholder));

  EXPECT_EQ(toolbar->GetCurrentText(), ConfigComboAdapter::kNoSelection);
  EXPECT_EQ(toolbar->GetCurrentIndex(), 0);
}

TEST_F(SplittableComboPanelTest, ButtonTextIsChangedWhenConfigDisplayNameChanges)
{
  ConfigurationModel model;
  auto configuration0 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration0->SetDisplayName("Config0");

  SplittableComboPanel widget;
  widget.show();

  const QSignalSpy selected_changed_spy(&widget,
                                        &SplittableComboPanel::selectedConfigurationChanged);
  widget.SetModel(&model);

  // select the configuration
  widget.SetItem(configuration0);
  ASSERT_EQ(selected_changed_spy.count(), 1);

  auto toolbar = widget.findChild<ComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);
  EXPECT_EQ(toolbar->GetCurrentText(), QString::fromStdString(configuration0->GetDisplayName()));
  EXPECT_EQ(toolbar->GetCurrentIndex(), 1);

  // change display name
  configuration0->SetDisplayName("NewConfig0");

  // expecting button text to be updated
  EXPECT_EQ(toolbar->GetCurrentText(), QString::fromStdString(configuration0->GetDisplayName()));

  ASSERT_EQ(selected_changed_spy.count(), 1);
}

}  // namespace sup::gui::test
