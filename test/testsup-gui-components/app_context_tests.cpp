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

#include "sup/gui/app/app_context.h"

#include <gtest/gtest.h>

#include <QWidget>

namespace sup::gui::test
{

class AppContextTest : public ::testing::Test
{
};

TEST_F(AppContextTest, MenuActionContainer)
{
  {
    const QString unique_id("abc");
    AppContext context(unique_id);
    EXPECT_EQ(context.GetContextId(), unique_id);
    EXPECT_TRUE(context.GetContextName().isEmpty());
  }

  {
    const QString unique_id("abc");
    const QString context_name("def");
    AppContext context(unique_id, context_name);
    EXPECT_EQ(context.GetContextId(), unique_id);
    EXPECT_EQ(context.GetContextName(), context_name);
  }
}

TEST_F(AppContextTest, EqualityOperators)
{
  const AppContext context1("Editor.Copy");
  EXPECT_TRUE(context1 == context1);
  EXPECT_FALSE(context1 != context1);

  const AppContext context2("Editor.Paste");
  EXPECT_FALSE(context1 == context2);
  EXPECT_TRUE(context1 != context2);

  const AppContext context3("Editor.Copy");
  EXPECT_TRUE(context1 == context3);
  EXPECT_FALSE(context1 != context3);
}

TEST_F(AppContextTest, ComparisonOperators)
{
  const AppContext context1("AAA");
  const AppContext context2("BBB");

  EXPECT_TRUE(context1 < context2);
  EXPECT_FALSE(context1 >= context2);
}

}  // namespace sup::gui::test
