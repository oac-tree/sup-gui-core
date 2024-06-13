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

#include "sup/gui/components/anyvalue_editor_action_handler.h"

#include <sup/gui/components/anyvalue_editor_context.h>
#include <sup/gui/components/anyvalue_editor_helper.h>
#include <sup/gui/components/mime_conversion_helper.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>

#include <sup/dto/anyvalue.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/test_utils.h>

#include <QMimeData>

using namespace sup::gui;
using ::testing::_;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

//! Tests for AnyValueEditorActionHandler for copy-and-paste scenario.

class AnyValueEditorActionHandlerCopyPasteTest : public ::testing::Test
{
public:
  /**
   * @brief Test helper to create context mimicking AnyValueEditor widget state.
   *
   * @param selection Currently selected item.
   * @param current_mime The content of the clipboard.
   */
  AnyValueEditorContext CreateContext(sup::gui::AnyValueItem* selection,
                                      const QMimeData* current_mime)
  {
    AnyValueEditorContext result;
    // callback returns given item, pretending it is user's selection
    result.get_selected_callback = [selection]() { return selection; };
    result.send_message_callback = m_warning_listener.AsStdFunction();
    result.get_mime_data = [current_mime]() { return current_mime; };
    result.set_mime_data = [this](std::unique_ptr<QMimeData> data)
    { m_copy_result = std::move(data); };
    return result;
  }

  /**
   * @brief Creates action handler which we will be testing.
   *
   * @param selection Currently selected item.
   * @param current_mime The content of the clipboard.
   */
  std::unique_ptr<AnyValueEditorActionHandler> CreateActionHandler(
      sup::gui::AnyValueItem* selection, const QMimeData* current_mime)
  {
    return std::make_unique<AnyValueEditorActionHandler>(CreateContext(selection, current_mime),
                                                         m_model.GetRootItem(), nullptr);
  }

  mvvm::SessionItem* GetAnyValueItemContainer() { return m_model.GetRootItem(); }

  mvvm::ApplicationModel m_model;
  testing::MockFunction<void(const sup::gui::MessageEvent&)> m_warning_listener;
  std::unique_ptr<QMimeData> m_copy_result;
};

//! Copy operation when nothing is selected, clipboard is empty.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, CopyPasteWhenNothingIsSelected)
{
  auto handler = CreateActionHandler(/*selected instruction*/ nullptr, /*mime*/ nullptr);

  EXPECT_FALSE(handler->CanCopy());
  EXPECT_FALSE(handler->CanPasteAfter());
  EXPECT_FALSE(handler->CanPasteInto());
  EXPECT_FALSE(handler->CanCut());
}

//! Copy operation when item is selected.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, CopyOperation)
{
  auto item = m_model.InsertItem<sup::gui::AnyValueStructItem>();

  EXPECT_EQ(m_copy_result.get(), nullptr);

  // AnyValueItem is selected, no mime
  auto handler = CreateActionHandler(item, nullptr);
  EXPECT_TRUE(handler->CanCopy());

  handler->Copy();

  // As a result of copy QMimeData object was created
  ASSERT_NE(m_copy_result.get(), nullptr);
  EXPECT_TRUE(m_copy_result->hasFormat(kCopyAnyValueMimeType));
}

//! Testing CanPasteAfter and CanPasteInto methods at different selections.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, CanPaste)
{
  {  // nothing is selected, no mime data
    auto handler = CreateActionHandler(/*selected instruction*/ nullptr, /*mime*/ nullptr);
    EXPECT_FALSE(handler->CanPasteAfter());
    EXPECT_FALSE(handler->CanPasteInto());
  }

  {  // nothing is selected, wrong mime data
    const QMimeData mime_data;
    auto handler = CreateActionHandler(nullptr, &mime_data);
    EXPECT_FALSE(handler->CanPasteAfter());
    EXPECT_FALSE(handler->CanPasteInto());
  }

  {  // nothing is selected, correct mime data
    const AnyValueStructItem item_to_paste;
    auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyAnyValueMimeType);
    auto handler = CreateActionHandler(nullptr, mime_data.get());

    // paste-after when nothing is selected is allowed
    EXPECT_TRUE(handler->CanPasteAfter());

    // paste-into when nothing is selected is forbidden
    EXPECT_FALSE(handler->CanPasteInto());
  }
}

//! Testing CanPasteAfter and CanPasteInto methods while trying to paste second top-level item.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, CanPasteSecondTopLevelItem)
{
  const AnyValueScalarItem item_to_paste;
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyAnyValueMimeType);

  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  auto handler = CreateActionHandler(parent, mime_data.get());
  EXPECT_FALSE(handler->CanPasteAfter());  // only one top-level item is allowed
  EXPECT_TRUE(handler->CanPasteInto());
}

//! Testing CanPasteAfter and CanPasteInto methods while trying to paste second top-level item.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, CanPasteIntoScalar)
{
  const AnyValueScalarItem item_to_paste;
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyAnyValueMimeType);

  auto parent = m_model.InsertItem<sup::gui::AnyValueScalarItem>();

  auto handler = CreateActionHandler(parent, mime_data.get());

  EXPECT_FALSE(handler->CanPasteAfter());
  EXPECT_FALSE(handler->CanPasteInto());
}

//! Testing CanPasteAfter and CanPasteInto when struct inside struct is selected.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, CanPasteStructIntoStruct)
{
  const AnyValueScalarItem item_to_paste;
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyAnyValueMimeType);

  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  auto child = m_model.InsertItem<sup::gui::AnyValueStructItem>(parent);

  auto handler = CreateActionHandler(child, mime_data.get());

  EXPECT_TRUE(handler->CanPasteAfter());
  EXPECT_TRUE(handler->CanPasteInto());
}
