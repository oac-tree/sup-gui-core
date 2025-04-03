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
#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>

#include <sup/dto/anyvalue.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace sup::gui;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(mvvm::SessionItem*)
#endif

/**
 * @brief Tests of AnyValueEditorActionHandlerUndoRedoTest class in undo/redo scenario.
 */
class AnyValueEditorActionHandlerUndoRedoTest : public ::testing::Test
{
public:
  AnyValueEditorActionHandlerUndoRedoTest()
  {
    m_container = m_model.InsertItem<mvvm::ContainerItem>();
  }

  /**
   * @brief Test helper to create context mimicking AnyValueEditor widget state.
   *
   * @param selection Currently selected item.
   */
  AnyValueEditorContext CreateContext(sup::gui::AnyValueItem* selection)
  {
    AnyValueEditorContext result;
    // callback returns given item, pretending it is user's selection
    result.selected_items = [selection]() { return std::vector<AnyValueItem*>({selection}); };
    result.send_message = m_warning_listener.AsStdFunction();
    return result;
  }

  /**
   * @brief Creates action handler which we will be testing.
   *
   * @param selection Currently selected item.
   */
  std::unique_ptr<AnyValueEditorActionHandler> CreateActionHandler(
      sup::gui::AnyValueItem* selection)
  {
    return std::make_unique<AnyValueEditorActionHandler>(CreateContext(selection), m_container,
                                                         nullptr);
  }

  /**
   * @brief Returns main container to store top-level AnyValueItem.
   */
  mvvm::SessionItem* GetContainer() const { return m_container; }

  mvvm::ApplicationModel m_model;
  mvvm::SessionItem* m_container{nullptr};
  testing::MockFunction<void(const sup::gui::MessageEvent&)> m_warning_listener;
};

TEST_F(AnyValueEditorActionHandlerUndoRedoTest, InitialState)
{
  auto handler = CreateActionHandler(nullptr);
  EXPECT_FALSE(handler->CanUndo());
  EXPECT_FALSE(handler->CanRedo());
}

TEST_F(AnyValueEditorActionHandlerUndoRedoTest, UndoRedoScenario)
{
  m_model.SetUndoEnabled(true);

  auto handler = CreateActionHandler(nullptr);
  EXPECT_FALSE(handler->CanUndo());
  EXPECT_FALSE(handler->CanRedo());

  // adding AnyValueItem struct as top level item
  handler->OnInsertAnyValueItemAfter(constants::kStructTypeName);
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);

  EXPECT_TRUE(handler->CanUndo());
  EXPECT_FALSE(handler->CanRedo());

  handler->Undo();
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 0);

  EXPECT_FALSE(handler->CanUndo());
  EXPECT_TRUE(handler->CanRedo());
}
