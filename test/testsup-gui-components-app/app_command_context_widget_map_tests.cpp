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

#include "sup/gui/app/app_command_context_widget_map.h"

#include <sup/gui/app/app_command_context.h>
#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <gtest/gtest.h>

#include <QWidget>

namespace sup::gui::test
{

/**
 * @brief Tests for AppCommandContextWidgetMap class.
 */
class AppCommandContextWidgetMapTest : public ::testing::Test
{
};

TEST_F(AppCommandContextWidgetMapTest, RegisterContext)
{
  AppCommandContextWidgetMap widget_map;
  const QWidget widget;

  // attempt to unregister non-registered widget
  EXPECT_THROW(widget_map.UnregisterWidgetUniqueId(&widget), RuntimeException);

  EXPECT_EQ(widget_map.GetNumberOfRegistrations(), 0);
  EXPECT_EQ(widget_map.GetContext(&widget), AppCommandContext{});
  EXPECT_FALSE(widget_map.HasContext(&widget));

  auto context1 = widget_map.RegisterWidgetUniqueId(&widget);
  EXPECT_TRUE(widget_map.HasContext(&widget));
  EXPECT_EQ(widget_map.GetContext(&widget), context1);
  EXPECT_EQ(widget_map.GetContext(&widget).GetContextName(), QString("QWidget"));
  EXPECT_EQ(widget_map.GetNumberOfRegistrations(), 1);

  // registering again same widget, context should conicide
  auto context2 = widget_map.RegisterWidgetUniqueId(&widget);
  EXPECT_NE(widget_map.GetContext(&widget), AppCommandContext{});
  EXPECT_EQ(context1, context2);

  // another widget
  const QWidget widget3;
  auto context3 = widget_map.RegisterWidgetUniqueId(&widget3);
  EXPECT_NE(context1, context3);

  // unregister widget
  widget_map.UnregisterWidgetUniqueId(&widget);
  widget_map.UnregisterWidgetUniqueId(&widget3);
  EXPECT_EQ(widget_map.GetNumberOfRegistrations(), 0);
}

}  // namespace sup::gui::test
