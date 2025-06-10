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

#include "sup/gui/components/abstract_text_content_controller.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for AbstractTextContentController class.
 */
class AbstractTextContentControllerTest : public ::testing::Test
{
public:
  /**
   * @brief The MockTextController class implements AbstractTextContentController interface and
   * validates its logic.
   */
  class MockTextController : public AbstractTextContentController
  {
  public:
    explicit MockTextController(mvvm::SessionItem* container)
        : AbstractTextContentController(container, CreateSendTextCallback(),
                                        CreateSendMessageCallback())
    {
    }

    /**
     * @brief Creates a callback to trigger own mock method and validate text sending.
     */
    AbstractTextContentController::send_text_func_t CreateSendTextCallback()
    {
      return [this](const auto& text) { OnSendText(text); };
    }

    /**
     * @brief Creates a callback to trigger own mock method and validate mesasge sending.
     */
    AbstractTextContentController::send_message_func_t CreateSendMessageCallback()
    {
      return [this](const auto& message) { OnSendMessage(message); };
    }

    MOCK_METHOD(std::string, GenerateText, (), (override));
    MOCK_METHOD(void, OnSendText, (const std::string&), ());
    MOCK_METHOD(void, OnSendMessage, (const MessageEvent&), ());
  };
};

TEST_F(AbstractTextContentControllerTest, SendTextOnPropertyChange)
{
  const std::string text1{"text1"};
  const std::string text2{"text2"};

  mvvm::ApplicationModel model;
  auto item = model.InsertItem<mvvm::SessionItem>();

  MockTextController controller(model.GetRootItem());

  ON_CALL(controller, GenerateText()).WillByDefault(::testing::Return(text1));

  EXPECT_CALL(controller, GenerateText()).Times(1);
  EXPECT_CALL(controller, OnSendText(text1)).Times(1);
  EXPECT_CALL(controller, OnSendMessage(::testing::_)).Times(0);
  item->SetData(42);

  // we do not send same text twice
  EXPECT_CALL(controller, GenerateText()).Times(1);
  EXPECT_CALL(controller, OnSendText(text1)).Times(0);
  EXPECT_CALL(controller, OnSendMessage(::testing::_)).Times(0);
  item->SetData(43);

  // changing text
  ON_CALL(controller, GenerateText()).WillByDefault(::testing::Return(text2));
  EXPECT_CALL(controller, GenerateText()).Times(1);
  EXPECT_CALL(controller, OnSendText(text2)).Times(1);
  EXPECT_CALL(controller, OnSendMessage(::testing::_)).Times(0);
  item->SetData(44);
}

TEST_F(AbstractTextContentControllerTest, SendTextOnDisplayNameChange)
{
  const std::string text1{"text1"};

  mvvm::ApplicationModel model;
  auto item = model.InsertItem<mvvm::SessionItem>();

  MockTextController controller(model.GetRootItem());

  ON_CALL(controller, GenerateText()).WillByDefault(::testing::Return(text1));

  EXPECT_CALL(controller, GenerateText()).Times(1);
  EXPECT_CALL(controller, OnSendText(text1)).Times(1);
  EXPECT_CALL(controller, OnSendMessage(::testing::_)).Times(0);
  item->SetDisplayName("abc");
}

TEST_F(AbstractTextContentControllerTest, GenerateTextWillThrow)
{
  const std::string text1{"text1"};

  mvvm::ApplicationModel model;
  auto item = model.InsertItem<mvvm::SessionItem>();

  MockTextController controller(model.GetRootItem());

  // on first model change will return text1
  ON_CALL(controller, GenerateText()).WillByDefault(::testing::Return(text1));

  EXPECT_CALL(controller, GenerateText()).Times(1);
  EXPECT_CALL(controller, OnSendText(text1)).Times(1);
  EXPECT_CALL(controller, OnSendMessage(::testing::_)).Times(0);
  item->SetDisplayName("abc");

  // on second model change will throw
  ON_CALL(controller, GenerateText())
      .WillByDefault(::testing::Throw(RuntimeException("error in text generation")));

  EXPECT_CALL(controller, GenerateText()).Times(1);
  EXPECT_CALL(controller, OnSendText(::testing::_)).Times(0);
  EXPECT_CALL(controller, OnSendMessage(::testing::_)).Times(1);
  item->SetDisplayName("def");

  // on third model change will return text1 again
  ON_CALL(controller, GenerateText()).WillByDefault(::testing::Return(text1));

  // we expect same text will be reported again
  EXPECT_CALL(controller, GenerateText()).Times(1);
  EXPECT_CALL(controller, OnSendText(text1)).Times(1);
  EXPECT_CALL(controller, OnSendMessage(::testing::_)).Times(0);
  item->SetDisplayName("abc");
}

}  // namespace sup::gui::test
