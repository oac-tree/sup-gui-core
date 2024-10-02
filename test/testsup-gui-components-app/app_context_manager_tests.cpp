/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sup/gui/app/app_context_manager.h"

#include <sup/gui/app/app_context.h>
#include <sup/gui/core/exceptions.h>

#include <gtest/gtest.h>

#include <QWidget>

using namespace sup::gui;

class AppContextManagerTest : public ::testing::Test
{
};

TEST_F(AppContextManagerTest, RegisterContext)
{
  AppContextManager manager;
  const QWidget widget;

  // attempt to unregister non-registered widget
  EXPECT_THROW(manager.UnregisterWidgetUniqueId(&widget), RuntimeException);

  EXPECT_EQ(manager.GetNumberOfRegistrations(), 0);
  EXPECT_EQ(manager.GetContext(&widget), AppContext{});
  EXPECT_FALSE(manager.HasContext(&widget));

  auto context1 = manager.RegisterWidgetUniqueId(&widget);
  EXPECT_TRUE(manager.HasContext(&widget));
  EXPECT_EQ(manager.GetContext(&widget), context1);
  EXPECT_EQ(manager.GetContext(&widget).GetContextName(), QString("QWidget"));
  EXPECT_EQ(manager.GetNumberOfRegistrations(), 1);

  // registering again same widget, context should conicide
  auto context2 = manager.RegisterWidgetUniqueId(&widget);
  EXPECT_NE(manager.GetContext(&widget), AppContext{});
  EXPECT_EQ(context1, context2);

  // another widget
  const QWidget widget3;
  auto context3 = manager.RegisterWidgetUniqueId(&widget3);
  EXPECT_NE(context1, context3);

  // unregister widget
  manager.UnregisterWidgetUniqueId(&widget);
  manager.UnregisterWidgetUniqueId(&widget3);
  EXPECT_EQ(manager.GetNumberOfRegistrations(), 0);
}
