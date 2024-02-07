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

#include "sup/gui/anyvalueeditor/anyvalue_editor_action_handler.h"

#include <sup/gui/anyvalueeditor/anyvalue_editor_context.h>
#include <sup/gui/anyvalueeditor/anyvalue_editor_helper.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/test/mock_callback_listener.h>
#include <mvvm/utils/file_utils.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/folder_based_test.h>
#include <testutils/test_utils.h>

using namespace sup::gui;
using ::testing::_;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

//! Extended tests for AnyValueEditorActionHandler.

class AnyValueEditorActionHandlerExtendedTest : public testutils::FolderBasedTest
{
public:
  AnyValueEditorActionHandlerExtendedTest()
      : testutils::FolderBasedTest("test_AnyValueEditorAction")
  {
  }

  //! Creates context necessary for AnyValueEditActions to function.
  AnyValueEditorContext CreateContext(sup::gui::AnyValueItem* item)
  {
    // callback returns given item, pretending it is user's selection
    auto get_selected_callback = [item]() { return item; };
    return {get_selected_callback, m_warning_listener.CreateCallback()};
  }

  //! Creates AnyValueEditorActions for testing.
  std::unique_ptr<AnyValueEditorActionHandler> CreateActionHandler(
      sup::gui::AnyValueItem* selection)
  {
    return std::make_unique<AnyValueEditorActionHandler>(CreateContext(selection),
                                                         m_model.GetRootItem(), nullptr);
  }

  mvvm::ApplicationModel m_model;
  mvvm::test::MockCallbackListener<sup::gui::MessageEvent> m_warning_listener;
};

TEST_F(AnyValueEditorActionHandlerExtendedTest, AddingArrayWithStructWithScalar)
{
  auto array_item = m_model.InsertItem<sup::gui::AnyValueArrayItem>();

  auto struct_item = m_model.InsertItem<sup::gui::AnyValueStructItem>(array_item);

  // creating action handler for the context, when struct is selected
  auto handler = CreateActionHandler(struct_item);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as a field.
  handler->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  EXPECT_EQ(struct_item->GetChildren().size(), 1);
};
