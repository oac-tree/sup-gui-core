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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sup/gui/core/sup_gui_core_exceptions.h"
#include "sup/gui/core/version_helper.h"

#include <gtest/gtest.h>

using namespace sup::gui;

class VersionHelperTest : public ::testing::Test
{
};

TEST_F(VersionHelperTest, ParseVersionString)
{
  EXPECT_EQ(ParseVersionString("1.2.3"), std::make_tuple(1, 2, 3));
  EXPECT_EQ(ParseVersionString(" 1. 2.3 "), std::make_tuple(1, 2, 3));
  EXPECT_THROW(ParseVersionString("1.2a.3patch"), RuntimeException);
  EXPECT_THROW(ParseVersionString("1..3"), RuntimeException);
  EXPECT_THROW(ParseVersionString("1.2."), RuntimeException);
  EXPECT_THROW(ParseVersionString("1.2"), RuntimeException);
}

TEST_F(VersionHelperTest, HasSameMajorMinorVersion)
{
  EXPECT_TRUE(HasSameMajorMinorVersion("1.2.0", "1.2.1"));
  EXPECT_FALSE(HasSameMajorMinorVersion("1.3.1", "1.2.2"));
}
