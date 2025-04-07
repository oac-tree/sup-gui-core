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
#include <sup/gui/components/mime_conversion_helper.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/mock_anyvalue_editor_context.h>

#include <QMimeData>

using namespace sup::gui;

/**
 * @brief Tests for AnyValueEditorActionHandle class in copy-and-paste scenario.
 */
class AnyValueEditorActionHandlerCopyPasteTest : public ::testing::Test
{
public:
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

  mvvm::SessionItem* GetContainer() { return m_model.GetRootItem(); }

  mvvm::ApplicationModel m_model;
  test::MockAnyValueEditorContext m_mock_context;
};

//! Copy operation when nothing is selected, clipboard is empty.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, CopyPasteWhenNothingIsSelected)
{
  auto handler = CreateActionHandler(/*selected instruction*/ {}, /*mime*/ {});

  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

  EXPECT_FALSE(handler->CanCopy());
  EXPECT_FALSE(handler->CanPasteAfter());
  EXPECT_FALSE(handler->CanPasteInto());
  EXPECT_FALSE(handler->CanCut());
}

TEST_F(AnyValueEditorActionHandlerCopyPasteTest, CopyOperationWhenItemIsSelected)
{
  auto item = m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // AnyValueItem is selected, no mime
  auto handler = CreateActionHandler({item}, nullptr);

  EXPECT_CALL(m_mock_context, OnSetMimeData()).Times(1);

  EXPECT_TRUE(handler->CanCopy());
  handler->Copy();

  // As a result of copy QMimeData object was created
  ASSERT_NE(m_mock_context.GetClipboardContent(), nullptr);
  EXPECT_TRUE(m_mock_context.GetClipboardContent()->hasFormat(kCopyAnyValueMimeType));
}

//! Testing CanPasteAfter and CanPasteInto methods at different selections.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, CanPaste)
{
  {  // nothing is selected, no mime data
    auto handler = CreateActionHandler(/*selected instruction*/ {}, /*mime*/ {});

    EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

    EXPECT_FALSE(handler->CanPasteAfter());
    EXPECT_FALSE(handler->CanPasteInto());
  }

  {  // nothing is selected, wrong mime data
    auto mime_data = std::make_unique<QMimeData>();
    auto handler = CreateActionHandler({}, std::move(mime_data));

    EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

    EXPECT_FALSE(handler->CanPasteAfter());
    EXPECT_FALSE(handler->CanPasteInto());
  }

  {  // nothing is selected, correct mime data
    const AnyValueStructItem item_to_paste;
    auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyAnyValueMimeType);
    auto handler = CreateActionHandler({}, std::move(mime_data));

    EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

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
  auto handler = CreateActionHandler({parent}, std::move(mime_data));

  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

  EXPECT_FALSE(handler->CanPasteAfter());  // only one top-level item is allowed
  EXPECT_TRUE(handler->CanPasteInto());
}

//! Testing CanPasteAfter and CanPasteInto methods while trying to paste into a scalar.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, CanPasteIntoScalar)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueScalarItem>();

  const AnyValueScalarItem item_to_paste;
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyAnyValueMimeType);
  auto handler = CreateActionHandler({parent}, std::move(mime_data));

  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

  EXPECT_FALSE(handler->CanPasteAfter());
  EXPECT_FALSE(handler->CanPasteInto());
}

//! Testing CanPasteAfter and CanPasteInto when struct inside struct is selected.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, CanPasteStructIntoStruct)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  auto child = m_model.InsertItem<sup::gui::AnyValueStructItem>(parent);

  const AnyValueScalarItem item_to_paste;
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyAnyValueMimeType);
  auto handler = CreateActionHandler({child}, std::move(mime_data));

  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);

  EXPECT_TRUE(handler->CanPasteAfter());
  EXPECT_TRUE(handler->CanPasteInto());
}

//! Testing PasteAfter operation, when container is empty.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, PasteAfterIntoEmptyContainer)
{
  AnyValueScalarItem item_to_paste;
  item_to_paste.SetDisplayName("abc");
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyAnyValueMimeType);

  // nothing is selected, copied item in a buffer
  auto handler = CreateActionHandler({}, std::move(mime_data));

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(3);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  EXPECT_TRUE(handler->CanPasteAfter());
  handler->PasteAfter();

  ASSERT_EQ(GetContainer()->GetTotalItemCount(), 1);

  auto items = GetContainer()->GetAllItems();
  EXPECT_EQ(items.at(0)->GetType(), AnyValueScalarItem::GetStaticType());

  // for the moment paste operation changes display name, it might change in the future
  EXPECT_EQ(items.at(0)->GetDisplayName(), constants::kAnyValueDefaultDisplayName);

  // validating request to select just inserted item
  EXPECT_EQ(m_mock_context.GetNotifyRequests(), std::vector<mvvm::SessionItem*>({items.at(0)}));
}

//! Testing PasteAfter operation when container has a struct with two fields. First field is
//! selected, paste operation should lead to appearance of a new field in between.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, PasteFieldInsideSequence)
{
  const std::string expected_field_name("abc");

  AnyValueScalarItem item_to_paste;
  item_to_paste.SetAnyTypeName(sup::dto::kInt16TypeName);
  item_to_paste.SetData(mvvm::int16{42});
  item_to_paste.SetDisplayName(expected_field_name);
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyAnyValueMimeType);

  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  auto field0 = parent->AddScalarField("field0", sup::dto::kInt32TypeName, mvvm::int32{0});
  auto field1 = parent->AddScalarField("field1", sup::dto::kInt32TypeName, mvvm::int32{1});

  // field0 is selected, copied item in a buffer
  auto handler = CreateActionHandler({field0}, std::move(mime_data));

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(3);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  EXPECT_TRUE(handler->CanPasteAfter());
  handler->PasteAfter();

  // validating that parent got new child
  ASSERT_EQ(parent->GetChildren().size(), 3);

  auto inserted_item = parent->GetChildren().at(1);

  // current implementation: pasted item will have the same field name as original display name
  EXPECT_EQ(inserted_item->GetDisplayName(), expected_field_name);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt16TypeName);
  EXPECT_EQ(inserted_item->GetToolTip(), sup::dto::kInt16TypeName);
  EXPECT_EQ(inserted_item->Data<mvvm::int16>(), 42);

  const std::vector<sup::gui::AnyValueItem*> expected_children({field0, inserted_item, field1});
  EXPECT_EQ(parent->GetChildren(), expected_children);

  // validating request to select just inserted item
  EXPECT_EQ(m_mock_context.GetNotifyRequests(), std::vector<mvvm::SessionItem*>({inserted_item}));
}

//! Testing PasteInto operation when container has a struct with single fields.
//! Struct itself is selected, paste-into a field will lead to field append.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, PasteIntoSequence)
{
  const std::string expected_field_name("abc");

  AnyValueScalarItem item_to_paste;
  item_to_paste.SetAnyTypeName(sup::dto::kInt16TypeName);
  item_to_paste.SetData(mvvm::int16{42});
  item_to_paste.SetDisplayName(expected_field_name);
  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kCopyAnyValueMimeType);

  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  auto field0 = parent->AddScalarField("field0", sup::dto::kInt32TypeName, mvvm::int32{0});

  // struct is selected, copied item in a buffer
  auto handler = CreateActionHandler({parent}, std::move(mime_data));

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(3);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  EXPECT_TRUE(handler->CanPasteInto());
  handler->PasteInto();

  // validating that parent got new child
  ASSERT_EQ(parent->GetChildren().size(), 2);

  auto inserted_item = parent->GetChildren().at(1);
  // current implementation: pasted item will have the same field name as original display name
  EXPECT_EQ(inserted_item->GetDisplayName(), expected_field_name);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt16TypeName);
  EXPECT_EQ(inserted_item->GetToolTip(), sup::dto::kInt16TypeName);
  EXPECT_EQ(inserted_item->Data<mvvm::int16>(), 42);

  const std::vector<sup::gui::AnyValueItem*> expected_children({field0, inserted_item});
  EXPECT_EQ(parent->GetChildren(), expected_children);

  // validating request to select just inserted item
  EXPECT_EQ(m_mock_context.GetNotifyRequests(), std::vector<mvvm::SessionItem*>({inserted_item}));
}

//! Testing Cut operation, when structure has two fields, first field is selected.
//! Cut operation should remove selected field.
TEST_F(AnyValueEditorActionHandlerCopyPasteTest, CutOperation)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  auto field0 = parent->AddScalarField("field0", sup::dto::kInt32TypeName, mvvm::int32{0});
  auto field1 = parent->AddScalarField("field1", sup::dto::kInt32TypeName, mvvm::int32{0});

  // struct is selected, mime buffer is empty
  auto handler = CreateActionHandler({field0}, {});

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, OnSetMimeData()).Times(1);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  EXPECT_TRUE(handler->CanCut());
  handler->Cut();

  const std::vector<sup::gui::AnyValueItem*> expected_children({field1});
  EXPECT_EQ(parent->GetChildren(), expected_children);

  // validating request to select remaining item
  EXPECT_EQ(m_mock_context.GetNotifyRequests(), std::vector<mvvm::SessionItem*>({field1}));

  // as a result of cut operation, QMimeData object was created
  ASSERT_NE(m_mock_context.GetClipboardContent(), nullptr);
  EXPECT_TRUE(m_mock_context.GetClipboardContent()->hasFormat(kCopyAnyValueMimeType));
  auto removed_field =
      CreateSessionItem(m_mock_context.GetClipboardContent(), kCopyAnyValueMimeType);
  EXPECT_EQ(removed_field->GetDisplayName(), std::string("field0"));
}

TEST_F(AnyValueEditorActionHandlerCopyPasteTest, CopyAndPasteBetweenTwoFields)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  auto field0 = parent->AddScalarField("field0", sup::dto::kInt32TypeName, mvvm::int32{42});
  auto field1 = parent->AddScalarField("field1", sup::dto::kInt32TypeName, mvvm::int32{43});

  // struct is selected, mime buffer is empty
  auto handler = CreateActionHandler({field0, field1}, {});

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, OnSetMimeData()).Times(1);
  EXPECT_CALL(m_mock_context, OnGetMimeData()).Times(2);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(2);

  handler->Copy();

  // pasting between field0 and field1
  m_mock_context.SetAsCurrentSelection({field0});

  handler->PasteAfter();

  ASSERT_EQ(parent->GetChildren().size(), 4);

  auto orig_field0 = parent->GetChildren().at(0);
  auto pasted_field0 = parent->GetChildren().at(1);
  auto pasted_field1 = parent->GetChildren().at(2);
  auto orig_field1 = parent->GetChildren().at(3);

  EXPECT_EQ(orig_field0, field0);
  EXPECT_EQ(orig_field1, field1);

  EXPECT_EQ(pasted_field0->GetDisplayName(), std::string("field0"));
  EXPECT_EQ(pasted_field0->Data<mvvm::int32>(), mvvm::int32{42});
  EXPECT_EQ(pasted_field1->GetDisplayName(), std::string("field1"));
  EXPECT_EQ(pasted_field1->Data<mvvm::int32>(), mvvm::int32{43});

  EXPECT_EQ(m_mock_context.GetNotifyRequests(),
            std::vector<mvvm::SessionItem*>({pasted_field0, pasted_field1}));
}
