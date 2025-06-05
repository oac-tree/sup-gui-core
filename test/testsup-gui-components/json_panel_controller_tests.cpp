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

#include "sup/gui/components/json_panel_controller.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/container_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for JsonPanelController class.
 */
class JsonPanelControllerTest : public ::testing::Test
{
public:
  JsonPanelControllerTest() { m_container = m_model.InsertItem<mvvm::ContainerItem>(); }

  std::unique_ptr<JsonPanelController> CreateController()
  {
    return std::make_unique<JsonPanelController>(m_container, m_mock_send_json.AsStdFunction(),
                                                 m_mock_send_message.AsStdFunction());
  }

  ::testing::MockFunction<JsonPanelController::send_json_func_t> m_mock_send_json;
  ::testing::MockFunction<JsonPanelController::send_message_func_t> m_mock_send_message;

  mvvm::SessionItem* m_container{nullptr};
  mvvm::ApplicationModel m_model;
};

TEST_F(JsonPanelControllerTest, AttemptToCreateConrtollerWithoutCallbacks)
{
  mvvm::SessionItem container;
  EXPECT_THROW(JsonPanelController(nullptr, {}, {}), RuntimeException);
  EXPECT_THROW(JsonPanelController(&container, {}, {}), RuntimeException);
  EXPECT_THROW(JsonPanelController(&container, [](auto) {}, {}), RuntimeException);
}

TEST_F(JsonPanelControllerTest, CheckInitialUpdate)
{
  EXPECT_CALL(m_mock_send_json, Call(std::string())).Times(1);
  auto controller = CreateController();
}

}  // namespace sup::gui::test
