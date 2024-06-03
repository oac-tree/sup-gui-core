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

  EXPECT_TRUE(manager.GetContext(&widget).empty());

  const AppContext context1("Editor.Copy");
  const AppContext context2("Editor.Paste");
  manager.RegisterContext(&widget, context1);
  manager.RegisterContext(&widget, context2);

  EXPECT_EQ(manager.GetContext(&widget), std::vector<AppContext>({context1, context2}));
}

TEST_F(AppContextManagerTest, RegisterWidgetUniqueId)
{
  AppContextManager manager;
  const QWidget widget;

  auto context = manager.RegisterWidgetUniqueId(&widget);

  EXPECT_EQ(context, manager.GetContext(&widget).at(0));
}
