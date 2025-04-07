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

#include <gtest/gtest.h>
#include <testutils/mock_anyvalue_editor_context.h>

#include <QMimeData>

using namespace sup::gui;

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
   * @brief Creates action handler which we will be testing.
   *
   * @param selection Currently selected items.
   * @param clipboard The content of the clipboard.
   */
  std::unique_ptr<AnyValueEditorActionHandler> CreateActionHandler(
      const std::vector<AnyValueItem*>& selection, std::unique_ptr<QMimeData> clipboard = {})
  {
    m_mock_context.SetClipboardContent(std::move(clipboard));
    return m_mock_context.CreateActionHandler(GetContainer(), selection);
  }

  /**
   * @brief Returns main container to store top-level AnyValueItem.
   */
  mvvm::SessionItem* GetContainer() const { return m_container; }

  mvvm::ApplicationModel m_model;
  mvvm::SessionItem* m_container{nullptr};
  test::MockAnyValueEditorContext m_mock_context;
};

TEST_F(AnyValueEditorActionHandlerUndoRedoTest, InitialState)
{
  auto handler = CreateActionHandler({});
  EXPECT_FALSE(handler->CanUndo());
  EXPECT_FALSE(handler->CanRedo());
}

TEST_F(AnyValueEditorActionHandlerUndoRedoTest, UndoRedoForItemInsert)
{
  m_model.SetUndoEnabled(true);

  auto handler = CreateActionHandler({});
  EXPECT_FALSE(handler->CanUndo());
  EXPECT_FALSE(handler->CanRedo());

  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  // adding AnyValueItem struct as top level item
  handler->InsertAnyValueItemAfter(constants::kStructTypeName);
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);

  EXPECT_TRUE(handler->CanUndo());
  EXPECT_FALSE(handler->CanRedo());

  handler->Undo();
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 0);

  EXPECT_FALSE(handler->CanUndo());
  EXPECT_TRUE(handler->CanRedo());

  handler->Redo();
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
}

TEST_F(AnyValueEditorActionHandlerUndoRedoTest, UndoRedoForCopyAndPasteTwoFields)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  auto field0 = parent->AddScalarField("field0", sup::dto::kInt32TypeName, mvvm::int32{42});
  auto field1 = parent->AddScalarField("field1", sup::dto::kInt32TypeName, mvvm::int32{43});

  m_model.SetUndoEnabled(true);

  auto handler = CreateActionHandler({field0, field1});

  EXPECT_FALSE(handler->CanUndo());
  EXPECT_FALSE(handler->CanRedo());

  EXPECT_CALL(m_mock_context, OnSetMimeData()).Times(1);
  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(2);

  handler->Copy();

  // pasting between field0 and field1
  m_mock_context.SetAsCurrentSelection({field0});

  EXPECT_FALSE(handler->CanUndo());
  EXPECT_FALSE(handler->CanRedo());

  handler->PasteAfter();
  EXPECT_EQ(parent->GetChildrenCount(), 4);

  EXPECT_TRUE(handler->CanUndo());
  EXPECT_FALSE(handler->CanRedo());

  handler->Undo();
  EXPECT_EQ(parent->GetChildrenCount(), 2);

  handler->Redo();
  EXPECT_EQ(parent->GetChildrenCount(), 4);
}
