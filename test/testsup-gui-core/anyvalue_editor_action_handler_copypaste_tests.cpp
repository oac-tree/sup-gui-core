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
