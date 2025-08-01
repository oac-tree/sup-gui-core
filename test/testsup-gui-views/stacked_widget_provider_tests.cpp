/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "sup/gui/mainwindow/stacked_widget_provider.h"

#include <gtest/gtest.h>

#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>

namespace sup::gui::test
{

/**
 * @brief Tests StackedWidgetProvider class
 */
class StackedWidgetProviderTest : public ::testing::Test
{
};

TEST_F(StackedWidgetProviderTest, InitialState)
{
  EXPECT_THROW(StackedWidgetProvider(nullptr), RuntimeException);

  QStackedWidget stacked_widget;
  StackedWidgetProvider provider(&stacked_widget);
  EXPECT_EQ(provider.GetStackedWidget(), &stacked_widget);

  EXPECT_THROW(provider.GetFactoryFunc("abc"), RuntimeException);
  EXPECT_THROW(provider.SetCurrentWidget("abc"), RuntimeException);
}

TEST_F(StackedWidgetProviderTest, RegisterWidget)
{
  QStackedWidget stacked_widget;
  StackedWidgetProvider provider(&stacked_widget);

  const std::string key1("key1");
  const std::string key2("key2");
  provider.RegisterWidget<QLabel>(key1);

  EXPECT_EQ(stacked_widget.currentWidget(), nullptr);

  auto label = provider.SetCurrentWidget(key1);
  EXPECT_NE(dynamic_cast<QLabel*>(label), nullptr);
  EXPECT_EQ(stacked_widget.currentWidget(), label);

  provider.RegisterWidget<QLineEdit>(key2);
  auto line_edit = provider.SetCurrentWidget(key2);
  EXPECT_EQ(stacked_widget.currentWidget(), line_edit);

  EXPECT_EQ(stacked_widget.currentIndex(), 1);
  EXPECT_EQ(stacked_widget.count(), 2);

  // reusing existing widget
  auto label2 = provider.SetCurrentWidget(key1);
  EXPECT_EQ(stacked_widget.currentIndex(), 0);
  EXPECT_EQ(stacked_widget.count(), 2);
  EXPECT_EQ(label2, label);
}

}  // namespace sup::gui::test
