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

#include "sup/gui/app/app_helper.h"

#include <gtest/gtest.h>

#include <QDebug>
#include <QSize>
#include <QString>

using namespace sup::gui;

/**
 * @brief Tests for helper methods located in app_helper.h
 */
class AppHelperTest : public ::testing::Test
{
};

TEST_F(AppHelperTest, ParseSizeString)
{
  EXPECT_EQ(ParseSizeString("1024x768").value_or(QSize()), QSize(1024, 768));
  EXPECT_EQ(ParseSizeString("1024 x 768").value_or(QSize()), QSize(1024, 768));
  EXPECT_EQ(ParseSizeString(" 24x25 ").value_or(QSize()), QSize(24, 25));

  EXPECT_FALSE(ParseSizeString("1024,768").has_value());
  EXPECT_FALSE(ParseSizeString("a1024x768").has_value());
  EXPECT_FALSE(ParseSizeString("x768").has_value());
  EXPECT_FALSE(ParseSizeString("x").has_value());
  EXPECT_FALSE(ParseSizeString("A0xB0").has_value());
  EXPECT_FALSE(ParseSizeString("41.1x42.2").has_value());
}
