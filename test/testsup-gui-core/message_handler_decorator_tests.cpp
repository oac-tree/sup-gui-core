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

#include "sup/gui/core/message_handler_decorator.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace sup::gui::test
{

class MessageHandlerDecoratorTest : public ::testing::Test
{
public:
  class MockMessageHandler : public IMessageHandler
  {
  public:
    MOCK_METHOD(void, SendMessage, (const MessageEvent& message), (override));
    MOCK_METHOD(void, ClearMessages, (), (override));
  };
};

TEST_F(MessageHandlerDecoratorTest, Decorate)
{
  MockMessageHandler mock_handler;

  EXPECT_CALL(mock_handler, SendMessage(::testing::_)).Times(1);

  MessageHandlerDecorator decorator(&mock_handler);
  decorator.SendMessage(MessageEvent{});

  EXPECT_CALL(mock_handler, ClearMessages()).Times(1);
  decorator.ClearMessages();
}

}  // namespace sup::gui::test
