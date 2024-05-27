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

#include "sup/gui/app/app_context.h"

#include <gtest/gtest.h>

#include <QWidget>

using namespace sup::gui;

class AppContextTest : public ::testing::Test
{
};

TEST_F(AppContextTest, MenuActionContainer)
{
  QWidget widget;

  AppContext context("Editor.Copy", &widget);
  EXPECT_EQ(context.GetContext(), QString("Editor.Copy"));
  EXPECT_EQ(context.GetFocusWidget(), &widget);
}
