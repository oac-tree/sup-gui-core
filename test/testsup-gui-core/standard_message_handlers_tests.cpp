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

#include "sup/gui/core/standard_message_handlers.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

class StandardMessageHandlersTest : public ::testing::Test
{
};

TEST_F(StandardMessageHandlersTest, StdMessageHandler)
{
  StdMessageHandler handler;
  testing::internal::CaptureStdout();
  EXPECT_NO_FATAL_FAILURE(
      handler.SendMessage(MessageEvent{"title", "text", "informative", "detailed"}));
  const std::string output = testing::internal::GetCapturedStdout();

  EXPECT_NE(output.find("title"), std::string::npos);
  EXPECT_NE(output.find("text"), std::string::npos);
  EXPECT_NE(output.find("informative"), std::string::npos);
  EXPECT_NE(output.find("detailed"), std::string::npos);
  EXPECT_NO_THROW(handler.ClearMessages());
}

TEST_F(StandardMessageHandlersTest, NullMessageHandler)
{
  NullMessageHandler handler;
  EXPECT_NO_FATAL_FAILURE(handler.SendMessage(MessageEvent{}));
  EXPECT_NO_THROW(handler.ClearMessages());
}

TEST_F(StandardMessageHandlersTest, ThrowingMessageHandler)
{
  ThrowingMessageHandler<RuntimeException> handler;
  EXPECT_THROW(handler.SendMessage(MessageEvent{"", "", "", "Exception details"}),
               RuntimeException);
  EXPECT_NO_THROW(handler.ClearMessages());
}

}  // namespace sup::gui::test
