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

#include "sup/gui/core/flags.h"

#include <gtest/gtest.h>

#include <QFlags>

using namespace sup::gui;

class FlagsTest : public ::testing::Test
{
public:
  enum class Option
  {
    NoOptions,
    kLength,
    kHeight,
    kWidth
  };
};

TEST_F(FlagsTest, BasicUsage)
{
  {
    const Flags<Option> flags({Option::kLength, Option::kHeight});
    EXPECT_TRUE(flags.HasFlag(Option::kLength));
    EXPECT_FALSE(flags.HasFlag(Option::kWidth));
  }

  {
    const Flags<Option> flags = Option::kLength;
    EXPECT_TRUE(flags.HasFlag(Option::kLength));
  }
}
