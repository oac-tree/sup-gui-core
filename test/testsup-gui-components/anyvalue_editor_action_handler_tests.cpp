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

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/mock_anyvalue_editor_context.h>

#include <QMimeData>

namespace sup::gui::test
{

/**
 * @brief Tests for AnyValueEditorActionHandler class in basic scenario.
 */
class AnyValueEditorActionHandlerTest : public ::testing::Test
{
public:
  AnyValueEditorActionHandlerTest() { m_container = m_model.InsertItem<mvvm::ContainerItem>(); }

  /**
   * @brief Creates action handler which we will be testing.
   *
   * @param selection Currently selected items.
   */
  std::unique_ptr<AnyValueEditorActionHandler> CreateActionHandler(
      const std::vector<AnyValueItem*>& selection)
  {
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

TEST_F(AnyValueEditorActionHandlerTest, AttemptToCreateWhenNoContextIsInitialised)
{
  {
    const AnyValueEditorContext context{};
    EXPECT_THROW((AnyValueEditorActionHandler{context, nullptr}), RuntimeException);
  }

  {
    AnyValueEditorContext context;
    context.selected_items = []() { return std::vector<AnyValueItem*>(); };
    EXPECT_THROW((AnyValueEditorActionHandler{context, nullptr}), RuntimeException);
  }

  {
    AnyValueEditorContext context;
    context.selected_items = []() { return std::vector<AnyValueItem*>(); };
    context.notify_request = [](auto item) { (void)item; };
    EXPECT_NO_THROW((AnyValueEditorActionHandler{context, nullptr}));
  }
}

TEST_F(AnyValueEditorActionHandlerTest, InitialState)
{
  auto handler = CreateActionHandler({});
  EXPECT_EQ(handler->GetTopItem(), nullptr);
  EXPECT_TRUE(handler->CanInsertAfter(constants::kStructTypeName));
  EXPECT_FALSE(handler->CanInsertInto(constants::kStructTypeName));
  EXPECT_FALSE(handler->CanRemove());
  EXPECT_FALSE(handler->CanInsertAfter(std::string()));
}

TEST_F(AnyValueEditorActionHandlerTest, HandlerWithNoContainerDefined)
{
  auto handler = std::make_unique<AnyValueEditorActionHandler>(m_mock_context.CreateContext({}));

  EXPECT_EQ(handler->GetTopItem(), nullptr);

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(2);

  handler->InsertAnyValueItemAfter(constants::kStructTypeName);
  handler->InsertAnyValueItemInto(constants::kStructTypeName);
}

//! Testing AnyValueEditorActions::SetInitialValue method.
TEST_F(AnyValueEditorActionHandlerTest, SetInitialValue)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(sup::dto::kInt32TypeName);
  item.SetData(42);

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
}

//! Testing AnyValueEditorActions::SetInitialValue method while trying to set the second initial
//! value.
TEST_F(AnyValueEditorActionHandlerTest, AttemptToSetInitialValueTwice)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(sup::dto::kInt32TypeName);
  item.SetData(42);

  auto handler = CreateActionHandler({});
  handler->SetInitialValue(item);
  ASSERT_NE(handler->GetTopItem(), nullptr);
  EXPECT_EQ(handler->GetTopItem()->GetIdentifier(), item.GetIdentifier());
  auto prev_top = handler->GetTopItem();

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  const AnyValueScalarItem item2;
  handler->SetInitialValue(item2);

  // expect oner warning, and still old top anyvalue
  EXPECT_EQ(handler->GetTopItem(), prev_top);
}

// -------------------------------------------------------------------------------------------------
// Adding empty
// -------------------------------------------------------------------------------------------------

//! Adding empty AnyValue to empty model
TEST_F(AnyValueEditorActionHandlerTest, OnAddEmptyAnyValueToEmptyModel)
{
  const std::string non_existing_type("non_existing_type");
  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler({});

  // expecting no warnings
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  EXPECT_TRUE(handler->CanInsertAfter(constants::kEmptyTypeName));
  EXPECT_FALSE(handler->CanInsertAfter(non_existing_type));
  EXPECT_FALSE(handler->CanInsertInto(constants::kEmptyTypeName));

  // adding empty AnyValueItem as top level item
  handler->InsertAnyValueItemAfter(constants::kEmptyTypeName);

  // validating that model got top level item of the correct type
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  auto inserted_item = GetContainer()->GetItem<AnyValueEmptyItem>(mvvm::TagIndex::First());
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), constants::kAnyValueDefaultDisplayName);

  EXPECT_EQ(m_mock_context.GetNotifyRequests(), std::vector<mvvm::SessionItem*>({inserted_item}));
};

// -------------------------------------------------------------------------------------------------
// Adding structures
// -------------------------------------------------------------------------------------------------

//! Adding structure to an empty model.
TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueStructToEmptyModel)
{
  const std::string non_existing_type("non_existing_type");
  // creating action for the context, when nothing is selected by the user
  auto handler = CreateActionHandler({});

  // expecting no warnings
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  EXPECT_TRUE(handler->CanInsertAfter(constants::kStructTypeName));
  EXPECT_FALSE(handler->CanInsertAfter(non_existing_type));
  EXPECT_FALSE(handler->CanInsertInto(constants::kStructTypeName));
  EXPECT_FALSE(handler->CanInsertAfter(non_existing_type));

  // adding AnyValueItem struct as top level item
  handler->InsertAnyValueItemAfter(constants::kStructTypeName);

  // validating that model got top level item of the correct type
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  auto inserted_item = GetContainer()->GetItem<AnyValueStructItem>(mvvm::TagIndex::First());
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), constants::kAnyValueDefaultDisplayName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), constants::kStructTypeName);

  EXPECT_EQ(m_mock_context.GetNotifyRequests(), std::vector<mvvm::SessionItem*>({inserted_item}));
};

//! Attempt to add a structure to a non-empty model when nothing is selected.
TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddToNonEmptyModel)
{
  // non-empty model
  m_model.InsertItem<sup::gui::AnyValueStructItem>(GetContainer(), mvvm::TagIndex::Append());

  // creating action for the context, when nothing is selected by the user
  auto handler = CreateActionHandler({});

  EXPECT_FALSE(handler->CanInsertAfter(constants::kStructTypeName));
  EXPECT_FALSE(handler->CanInsertInto(constants::kStructTypeName));

  // expecting warning
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // attempt to add another top level item
  handler->InsertAnyValueItemAfter(constants::kStructTypeName);

  // validating that there is still one item
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
};

//! Attempt to add a second structure after top-level selected structure.
TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddSecondTopLevelStructure)
{
  // non-empty model
  auto parent =
      m_model.InsertItem<sup::gui::AnyValueStructItem>(GetContainer(), mvvm::TagIndex::Append());

  // creating action for the context, when nothing is selected by the user
  auto handler = CreateActionHandler({parent});

  EXPECT_TRUE(handler->CanRemove());

  EXPECT_FALSE(handler->CanInsertAfter(constants::kStructTypeName));

  // expecting warning
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // attempt to add another top level item
  handler->InsertAnyValueItemAfter(constants::kStructTypeName);

  // validating that there is still one item
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
};

//! Adding structure as a field to another structure (which is marked as selected).
TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueStructToAnotherStruct)
{
  auto parent =
      m_model.InsertItem<sup::gui::AnyValueStructItem>(GetContainer(), mvvm::TagIndex::Append());

  // creating action for the context, when parent is selected
  auto handler = CreateActionHandler({parent});

  // expecting no callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  EXPECT_TRUE(handler->CanInsertInto(constants::kStructTypeName));

  // adding AnyValueItem struct as a field
  handler->InsertAnyValueItemInto(constants::kStructTypeName);

  // validating that parent got new child
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  EXPECT_EQ(inserted_item->GetType(), std::string("AnyValueStruct"));

  const std::string expected_field_name(constants::kFieldNamePrefix + "0");
  EXPECT_EQ(inserted_item->GetDisplayName(), expected_field_name);
};

//! Attempt to add a structure after a scalar.
TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddStructToScalar)
{
  auto parent =
      m_model.InsertItem<sup::gui::AnyValueScalarItem>(GetContainer(), mvvm::TagIndex::Append());

  // creating action for the context, when parent is selected
  auto handler = CreateActionHandler({parent});

  // expecting error callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  EXPECT_FALSE(handler->CanInsertAfter(constants::kStructTypeName));

  // adding AnyValueItem struct as a field to
  handler->InsertAnyValueItemAfter(constants::kStructTypeName);

  // validating that nothing can changed in the model
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 0);
};

//-------------------------------------------------------------------------------------------------
// Adding scalars
//-------------------------------------------------------------------------------------------------

//! Adding a scalar to an empty model.
TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueScalarToEmptyModel)
{
  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler({});

  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  // adding AnyValueItem struct as top level item
  handler->InsertAnyValueItemAfter(sup::dto::kInt32TypeName);

  // validating that model got top level item of the correct type
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  auto inserted_item = GetContainer()->GetItem<AnyValueScalarItem>(mvvm::TagIndex::First());
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), constants::kAnyValueDefaultDisplayName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);

  EXPECT_FALSE(handler->CanInsertAfter(constants::kStructTypeName));

  // expecting warning callback further down
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // adding another scalar when nothing is selected should trigger the warning
  handler->InsertAnyValueItemAfter(sup::dto::kInt32TypeName);

  // the amount of items should stay the same
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
};

//! Adding scalar as a field to another structure (which is marked as selected).
TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueScalarToStruct)
{
  auto parent =
      m_model.InsertItem<sup::gui::AnyValueStructItem>(GetContainer(), mvvm::TagIndex::Append());

  // creating action handler for the context, when parent is selected
  auto handler = CreateActionHandler({parent});

  EXPECT_TRUE(handler->CanInsertInto(sup::dto::kInt32TypeName));

  // expecting no callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  // adding AnyValueItem struct as a field
  handler->InsertAnyValueItemInto(sup::dto::kInt32TypeName);

  // validating that parent got new child
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  const std::string expected_field_name(constants::kFieldNamePrefix + "0");
  EXPECT_EQ(inserted_item->GetDisplayName(), expected_field_name);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->GetToolTip(), sup::dto::kInt32TypeName);
};

//! Structure has two fields, first field is selected. Adding new field between two existing ones.
TEST_F(AnyValueEditorActionHandlerTest, InsertFieldInStruct)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  auto field0 = parent->AddScalarField("field0", sup::dto::kInt32TypeName, 42);
  auto field1 = parent->AddScalarField("field1", sup::dto::kInt32TypeName, 42);

  // creating action handler for the context, when field0 is selected
  auto handler = CreateActionHandler({field0});

  // expecting no callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  // adding AnyValueItem struct as a field
  handler->InsertAnyValueItemAfter(sup::dto::kInt32TypeName);

  // validating that parent got new child
  ASSERT_EQ(parent->GetChildren().size(), 3);

  auto inserted_item = parent->GetChildren().at(1);
  const std::string expected_field_name(constants::kFieldNamePrefix + "2");
  EXPECT_EQ(inserted_item->GetDisplayName(), expected_field_name);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->GetToolTip(), sup::dto::kInt32TypeName);

  const std::vector<sup::gui::AnyValueItem*> expected_children({field0, inserted_item, field1});
  EXPECT_EQ(parent->GetChildren(), expected_children);
};

//! Adding a scalar as an array element (which is marked as selected).
TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueScalarToArray)
{
  auto parent =
      m_model.InsertItem<sup::gui::AnyValueArrayItem>(GetContainer(), mvvm::TagIndex::Append());

  // creating action handler for the context, when parent is selected
  auto handler = CreateActionHandler({parent});

  EXPECT_TRUE(handler->CanInsertInto(sup::dto::kInt32TypeName));

  // expecting no callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  // adding AnyValueItem struct as a field
  handler->InsertAnyValueItemInto(sup::dto::kInt32TypeName);

  // validating that parent got new child
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  const std::string expected_field_name(constants::kElementNamePrefix + "0");
  EXPECT_EQ(inserted_item->GetDisplayName(), expected_field_name);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->GetToolTip(), sup::dto::kInt32TypeName);
};

//! Attempt to add scalar as a field to another scalar.
TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddScalarToScalar)
{
  auto parent =
      m_model.InsertItem<sup::gui::AnyValueScalarItem>(GetContainer(), mvvm::TagIndex::Append());

  // creating action handler for the context, when parent is selected
  auto handler = CreateActionHandler({parent});

  EXPECT_FALSE(handler->CanInsertInto(sup::dto::kInt32TypeName));

  // expecting no callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // adding AnyValueItem struct as a field
  handler->InsertAnyValueItemAfter(sup::dto::kInt32TypeName);

  // validating that nothing was changed in the model
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 0);
};

//! Attempt to add second top level scalar to the model.
TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddSecondTopLevelScalar)
{
  m_model.InsertItem<sup::gui::AnyValueScalarItem>(GetContainer(), mvvm::TagIndex::Append());

  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler({nullptr});

  EXPECT_FALSE(handler->CanInsertAfter(sup::dto::kInt32TypeName));

  // expecting warning callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // attempt to add second top level scalar
  handler->InsertAnyValueItemAfter(sup::dto::kInt32TypeName);

  // checking that model still have a single item
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
};

TEST_F(AnyValueEditorActionHandlerTest, AddScalarToArrayWhenTypeMismath)
{
  auto parent =
      m_model.InsertItem<sup::gui::AnyValueArrayItem>(GetContainer(), mvvm::TagIndex::Append());
  auto element0 = m_model.InsertItem<sup::gui::AnyValueScalarItem>(parent);
  element0->SetAnyTypeName(sup::dto::kInt32TypeName);

  // creating action handler for the context, when parent is selected
  auto handler = CreateActionHandler({element0});

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);

  // adding AnyValueItem scalar as a field. The type matches what is already in the array.
  handler->InsertAnyValueItemAfter(sup::dto::kInt32TypeName);

  // validating that parent got new child
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 2);

  // We allow adding mixed types to the array. We assume, that user will fix them before the use.
  handler->InsertAnyValueItemAfter(sup::dto::kInt16TypeName);

  EXPECT_EQ(parent->GetChildren().size(), 3);
};

//-------------------------------------------------------------------------------------------------
// Adding array
//-------------------------------------------------------------------------------------------------

//! Adding an array to an empty model.
TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueArrayToEmptyModel)
{
  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler({nullptr});

  EXPECT_TRUE(handler->CanInsertAfter(constants::kArrayTypeName));

  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  // adding AnyValueItem struct as top level item
  handler->InsertAnyValueItemAfter(constants::kArrayTypeName);

  // validating that model got top level item of the correct type
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  auto inserted_item = GetContainer()->GetItem<AnyValueArrayItem>(mvvm::TagIndex::First());
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), constants::kAnyValueDefaultDisplayName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), constants::kArrayTypeName);

  // expecting a callback
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // attempt to add second top-level item
  handler->InsertAnyValueItemAfter(constants::kArrayTypeName);

  // the amount of items should stay the same
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
};

//! Adding array as a field to another structure (which is marked as selected).
TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueArrayToStruct)
{
  auto parent =
      m_model.InsertItem<sup::gui::AnyValueStructItem>(GetContainer(), mvvm::TagIndex::Append());

  // creating action handler for the context, when parent is selected
  auto handler = CreateActionHandler({parent});

  EXPECT_TRUE(handler->CanInsertInto(constants::kArrayTypeName));

  // expecting no callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  // adding AnyValueItem struct as a field
  handler->InsertAnyValueItemInto(constants::kArrayTypeName);

  // validating that parent got new child
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  EXPECT_EQ(inserted_item->GetType(), std::string("AnyValueArray"));
  const std::string expected_field_name(constants::kFieldNamePrefix + "0");
  EXPECT_EQ(inserted_item->GetDisplayName(), expected_field_name);
};

//! Attempt to add array as a field to a scalar.
TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddArrayToScalar)
{
  auto parent =
      m_model.InsertItem<sup::gui::AnyValueScalarItem>(GetContainer(), mvvm::TagIndex::Append());

  // creating action handler for the context, when parent is selected
  auto handler = CreateActionHandler({parent});

  EXPECT_FALSE(handler->CanInsertAfter(constants::kArrayTypeName));

  // expecting error callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // adding AnyValueItem struct as a field to
  handler->InsertAnyValueItemAfter(constants::kArrayTypeName);

  // validating that nothing can changed in the model
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 0);
};

//! Attempt to add second top level array to the model.
TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddSecondTopLevelArray)
{
  m_model.InsertItem<sup::gui::AnyValueArrayItem>(GetContainer(), mvvm::TagIndex::Append());

  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler({nullptr});

  EXPECT_FALSE(handler->CanInsertAfter(constants::kArrayTypeName));

  // expecting warning callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // attempt to add second top level array
  handler->InsertAnyValueItemAfter(constants::kArrayTypeName);

  // checking that model still have a single item
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
};

//-------------------------------------------------------------------------------------------------
// Remove item
//-------------------------------------------------------------------------------------------------

TEST_F(AnyValueEditorActionHandlerTest, RemoveItemWhenNothingIsSelected)
{
  auto struct_item =
      m_model.InsertItem<sup::gui::AnyValueStructItem>(GetContainer(), mvvm::TagIndex::Append());

  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler({nullptr});

  EXPECT_FALSE(handler->CanRemove());
  EXPECT_NO_FATAL_FAILURE(handler->RemoveSelected());

  // validating that still has an item
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
};

TEST_F(AnyValueEditorActionHandlerTest, RemoveSelectedItem)
{
  auto struct_item =
      m_model.InsertItem<sup::gui::AnyValueStructItem>(GetContainer(), mvvm::TagIndex::Append());
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);

  // creating action handler for the context, pretending item is selected
  auto handler = CreateActionHandler({struct_item});

  EXPECT_TRUE(handler->CanRemove());

  handler->RemoveSelected();

  // validating that there is no item anymore
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 0);
};

TEST_F(AnyValueEditorActionHandlerTest, RemoveFieldsInTheMiddle)
{
  auto parent =
      m_model.InsertItem<sup::gui::AnyValueStructItem>(GetContainer(), mvvm::TagIndex::Append());
  auto field0 = parent->AddScalarField("field0", sup::dto::kInt32TypeName, mvvm::int32{42});
  auto field1 = parent->AddScalarField("field1", sup::dto::kInt32TypeName, mvvm::int32{43});
  auto field2 = parent->AddScalarField("field2", sup::dto::kInt32TypeName, mvvm::int32{44});
  auto field3 = parent->AddScalarField("field3", sup::dto::kInt32TypeName, mvvm::int32{45});

  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);

  auto handler = CreateActionHandler({field1, field2});

  EXPECT_TRUE(handler->CanRemove());

  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  handler->RemoveSelected();

  EXPECT_EQ(parent->GetChildren(), std::vector<AnyValueItem*>({field0, field3}));
  EXPECT_EQ(m_mock_context.GetNotifyRequests(), std::vector<mvvm::SessionItem*>({field3}));
};

TEST_F(AnyValueEditorActionHandlerTest, RemoveParentWhenChildIsSelectedToo)
{
  auto parent =
      m_model.InsertItem<sup::gui::AnyValueStructItem>(GetContainer(), mvvm::TagIndex::Append());
  auto field0 = parent->AddScalarField("field0", sup::dto::kInt32TypeName, mvvm::int32{42});
  auto field1 = parent->AddScalarField("field1", sup::dto::kInt32TypeName, mvvm::int32{43});

  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);

  auto handler = CreateActionHandler({field1, parent});

  EXPECT_TRUE(handler->CanRemove());

  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(0);

  handler->RemoveSelected();

  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 0);
};

//-------------------------------------------------------------------------------------------------
// Move Up/Down
//-------------------------------------------------------------------------------------------------

TEST_F(AnyValueEditorActionHandlerTest, MoveUp)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  auto field0 = parent->AddScalarField("field0", sup::dto::kInt32TypeName, mvvm::int32{42});
  auto field1 = parent->AddScalarField("field1", sup::dto::kInt32TypeName, mvvm::int32{43});

  auto handler = CreateActionHandler({field1});

  // expecting no callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  // moving selected item up
  handler->MoveUp();

  // validating that parent got new child
  EXPECT_EQ(parent->GetChildren(), std::vector<sup::gui::AnyValueItem*>({field1, field0}));

  EXPECT_EQ(m_mock_context.GetNotifyRequests(), std::vector<mvvm::SessionItem*>({field1}));

  // moving selected item up second time doesn't change anything
  handler->MoveUp();

  EXPECT_EQ(parent->GetChildren(), std::vector<sup::gui::AnyValueItem*>({field1, field0}));
  EXPECT_EQ(m_mock_context.GetNotifyRequests(), std::vector<mvvm::SessionItem*>({field1}));
};

TEST_F(AnyValueEditorActionHandlerTest, MoveDown)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  auto field0 = parent->AddScalarField("field0", sup::dto::kInt32TypeName, mvvm::int32{42});
  auto field1 = parent->AddScalarField("field1", sup::dto::kInt32TypeName, mvvm::int32{43});

  // creating action handler for the context, when field1 is selected
  auto handler = CreateActionHandler({field0});

  // expecting no callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, NotifyRequest(::testing::_)).Times(1);

  // moving selected item up
  handler->MoveDown();

  // validating that parent got new child
  EXPECT_EQ(parent->GetChildren(), std::vector<sup::gui::AnyValueItem*>({field1, field0}));

  EXPECT_EQ(m_mock_context.GetNotifyRequests(), std::vector<mvvm::SessionItem*>({field0}));

  // moving selected item up second time doesn't change anything
  handler->MoveDown();

  EXPECT_EQ(parent->GetChildren(), std::vector<sup::gui::AnyValueItem*>({field1, field0}));
  EXPECT_EQ(m_mock_context.GetNotifyRequests(), std::vector<mvvm::SessionItem*>({field0}));
};

}  // namespace sup::gui::test
