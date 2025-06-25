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

#include "sup/gui/components/anyvalue_editor_action_handler.h"

#include <sup/gui/components/anyvalue_editor_context.h>
#include <sup/gui/components/anyvalue_editor_helper.h>
#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/utils/file_utils.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>
#include <testutils/mock_anyvalue_editor_context.h>

#include <QMimeData>

namespace sup::gui::test
{

/**
 * @brief Tests for AnyValueEditorActionHandle class covering more scenarios.
 */
class AnyValueEditorActionHandlerExtendedTest : public test::FolderTest
{
public:
  AnyValueEditorActionHandlerExtendedTest()
      : test::FolderTest("AnyValueEditorActionHandlerExtendedTest")
  {
  }
  /**
   * @brief Creates action handler which we will be testing.
   *
   * @param selection Currently selected items.
   * @param clipboard The content of the clipboard.
   */
  std::unique_ptr<AnyValueEditorActionHandler> CreateActionHandler(
      const std::vector<AnyValueItem*>& selection, std::unique_ptr<QMimeData> clipboard = {})
  {
    m_mock_context.SetClipboardContent(std::move(clipboard));
    return m_mock_context.CreateActionHandler(m_model.GetRootItem(), selection);
  }

  mvvm::ApplicationModel m_model;
  test::MockAnyValueEditorContext m_mock_context;
};

TEST_F(AnyValueEditorActionHandlerExtendedTest, AddingArrayWithStructWithScalar)
{
  auto array_item = m_model.InsertItem<sup::gui::AnyValueArrayItem>();

  auto struct_item = m_model.InsertItem<sup::gui::AnyValueStructItem>(array_item);

  // creating action handler for the context, when struct is selected
  auto handler = CreateActionHandler({struct_item});

  EXPECT_TRUE(handler->CanInsertInto(sup::dto::kInt32TypeName));

  // expecting no callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  // adding AnyValueItem struct as a field.
  handler->InsertAnyValueItemInto(sup::dto::kInt32TypeName);

  EXPECT_EQ(struct_item->GetChildren().size(), 1);
};

//! Validating that editor removes all hidden/disabled flags from initial item.

TEST_F(AnyValueEditorActionHandlerExtendedTest, SetInitialValueMarkedAsDisabled)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(sup::dto::kInt32TypeName);
  item.SetData(42);
  item.SetEnabled(false);
  item.SetEditable(false);

  // expecting no callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);

  auto handler = CreateActionHandler({});
  handler->SetInitialValue(item);
  ASSERT_NE(handler->GetTopItem(), nullptr);

  // validating
  auto copied_item = dynamic_cast<AnyValueScalarItem*>(handler->GetTopItem());
  ASSERT_NE(copied_item, nullptr);
  EXPECT_NE(copied_item, &item);
  EXPECT_EQ(copied_item->GetAnyTypeName(), item.GetAnyTypeName());
  EXPECT_EQ(copied_item->Data<int>(), item.Data<int>());
  EXPECT_EQ(copied_item->GetIdentifier(), item.GetIdentifier());
  EXPECT_TRUE(copied_item->IsEditable());
  EXPECT_TRUE(copied_item->IsEnabled());
}

TEST_F(AnyValueEditorActionHandlerExtendedTest, AttemptToInsertWhenModelIsAbsent)
{
  mvvm::ContainerItem container;  // no model
  auto handler = m_mock_context.CreateActionHandler(&container, {});

  // expecting no warnings
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  // EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  EXPECT_TRUE(handler->CanInsertAfter(constants::kStructTypeName));
  EXPECT_FALSE(handler->CanInsertInto(constants::kStructTypeName));

  // adding AnyValueItem struct as top level item
  EXPECT_THROW(handler->InsertAnyValueItemAfter(constants::kStructTypeName), RuntimeException);
};

}  // namespace sup::gui::test
