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
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/container_item.h>

#include <sup/dto/anytype.h>

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

  ::testing::MockFunction<JsonPanelController::send_text_func_t> m_mock_send_json;
  ::testing::MockFunction<JsonPanelController::send_message_func_t> m_mock_send_message;

  mvvm::ContainerItem* m_container{nullptr};
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

TEST_F(JsonPanelControllerTest, NotificationOnContainerRemoval)
{
  EXPECT_CALL(m_mock_send_json, Call(::testing::_)).Times(1);
  auto controller = CreateController();

  EXPECT_CALL(m_mock_send_json, Call(std::string())).Times(0);
  m_model.RemoveItem(m_container);
}

TEST_F(JsonPanelControllerTest, InitialJsonUpdate)
{
  m_model.InsertItem<AnyValueStructItem>(m_container, mvvm::TagIndex::Append());

  const std::string expected_json(
      R"RAW([{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"","attributes":[]}},{"instance":{}}])RAW");

  EXPECT_CALL(m_mock_send_json, Call(expected_json)).Times(1);
  auto controller = CreateController();
}

TEST_F(JsonPanelControllerTest, JsonUpdateOnItemInsert)
{
  const std::string expected_json(
      R"RAW([{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"","attributes":[]}},{"instance":{}}])RAW");

  EXPECT_CALL(m_mock_send_json, Call(std::string())).Times(1);
  auto controller = CreateController();

  EXPECT_CALL(m_mock_send_json, Call(expected_json)).Times(1);
  m_model.InsertItem<AnyValueStructItem>(m_container, mvvm::TagIndex::Append());
}

TEST_F(JsonPanelControllerTest, JsonUpdateOnItemDeletion)
{
  m_model.InsertItem<AnyValueStructItem>(m_container, mvvm::TagIndex::Append());

  const std::string expected_json(
      R"RAW([{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"","attributes":[]}},{"instance":{}}])RAW");

  EXPECT_CALL(m_mock_send_json, Call(expected_json)).Times(1);
  auto controller = CreateController();

  EXPECT_CALL(m_mock_send_json, Call(std::string())).Times(1);
  m_container->Clear();
}

TEST_F(JsonPanelControllerTest, JsonUpdateOnScalarInsert)
{
  const std::string expected_json1(
      R"RAW([{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"","attributes":[]}},{"instance":{}}])RAW");

  EXPECT_CALL(m_mock_send_json, Call(std::string())).Times(1);
  auto controller = CreateController();

  EXPECT_CALL(m_mock_send_json, Call(expected_json1)).Times(1);
  auto struct_item = m_model.InsertItem<AnyValueStructItem>(m_container, mvvm::TagIndex::Append());

  const std::string expected_json2(
      R"RAW([{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"struct","attributes":[]}},{"instance":{}}])RAW");
  EXPECT_CALL(m_mock_send_json, Call(expected_json2)).Times(1);
  struct_item->SetAnyTypeName("struct");
}

TEST_F(JsonPanelControllerTest, CheckErrorMessagesOnScalarTypeChange)
{
  auto scalar_item = m_model.InsertItem<AnyValueScalarItem>(m_container, mvvm::TagIndex::Append());
  scalar_item->SetAnyTypeName(sup::dto::kInt32TypeName);

  const std::string expected_json1(
      R"RAW([{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"int32"}},{"instance":0}])RAW");

  EXPECT_CALL(m_mock_send_json, Call(expected_json1)).Times(1);
  auto controller = CreateController();

  // changing type
  const std::string expected_json2(
      R"RAW([{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"bool"}},{"instance":false}])RAW");

  {
    const ::testing::InSequence seq;

    // old variant vas replaced with empty variant, that triggered a failure in
    // sup::gui::CreateAnyValue
    EXPECT_CALL(m_mock_send_message, Call(::testing::_)).Times(1);

    // empty variant was replaced with bool variant
    EXPECT_CALL(m_mock_send_json, Call(expected_json2)).Times(1);
  }
  scalar_item->SetAnyTypeName(sup::dto::kBooleanTypeName);
}

TEST_F(JsonPanelControllerTest, JsonUpdateOnPrettyChange)
{
  auto scalar_item = m_model.InsertItem<AnyValueScalarItem>(m_container, mvvm::TagIndex::Append());
  scalar_item->SetAnyTypeName(sup::dto::kInt32TypeName);

  const std::string expected_json1(
      R"RAW([{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"int32"}},{"instance":0}])RAW");

  EXPECT_CALL(m_mock_send_json, Call(expected_json1)).Times(1);
  auto controller = CreateController();

  const std::string expected_json2(
      R"RAW([
  {
    "encoding": "sup-dto/v1.0/JSON"
  },
  {
    "datatype": {
      "type": "int32"
    }
  },
  {
    "instance": 0
  }
])RAW");

  EXPECT_CALL(m_mock_send_json, Call(expected_json2)).Times(1);
  controller->SetPrettyJson(true);
}

}  // namespace sup::gui::test
