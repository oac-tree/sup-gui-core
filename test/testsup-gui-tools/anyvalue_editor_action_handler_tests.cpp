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

#include <QSignalSpy>

using namespace sup::gui;
using ::testing::_;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

//! Tests for AnyValueEditorActionHandler.

class AnyValueEditorActionHandlerTest : public testutils::FolderBasedTest
{
public:
  AnyValueEditorActionHandlerTest() : testutils::FolderBasedTest("test_AnyValueEditorAction") {}

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
    return std::make_unique<AnyValueEditorActionHandler>(CreateContext(selection), &m_model,
                                                         nullptr);
  }

  mvvm::SessionItem* GetAnyValueItemContainer() { return m_model.GetRootItem(); }

  mvvm::ApplicationModel m_model;
  mvvm::test::MockCallbackListener<sup::gui::MessageEvent> m_warning_listener;
};

//! Testing initial state of AnyValueEditorActions object.

TEST_F(AnyValueEditorActionHandlerTest, InitialState)
{
  auto actions = CreateActionHandler(nullptr);
  EXPECT_EQ(actions->GetTopItem(), nullptr);
  EXPECT_EQ(actions->GetSelectedItem(), nullptr);
  EXPECT_EQ(GetAnyValueItemContainer(), actions->GetAnyValueItemContainer());
}

//! Testing AnyValueEditorActions::SetInitialValue method.

TEST_F(AnyValueEditorActionHandlerTest, SetInitialValue)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(sup::dto::kInt32TypeName);
  item.SetData(42);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  auto handler = CreateActionHandler(nullptr);
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

  auto handler = CreateActionHandler(nullptr);
  handler->SetInitialValue(item);
  ASSERT_NE(handler->GetTopItem(), nullptr);
  EXPECT_EQ(handler->GetTopItem()->GetIdentifier(), item.GetIdentifier());
  auto prev_top = handler->GetTopItem();

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  AnyValueScalarItem item2;
  handler->SetInitialValue(item2);

  // expect oner warning, and still old top anyvalue
  EXPECT_EQ(handler->GetTopItem(), prev_top);
}

// -------------------------------------------------------------------------------------------------
// Adding empty
// -------------------------------------------------------------------------------------------------

//! Adding empty AnyValue to empty model

TEST_F(AnyValueEditorActionHandlerTest, OnAddEmptyAnyValueStructToEmptyModel)
{
  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler(nullptr);

  QSignalSpy spy_selection_request(handler.get(), &AnyValueEditorActionHandler::SelectItemRequest);

  EXPECT_EQ(handler->GetSelectedItem(), nullptr);

  // expecting no warnings
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding empty AnyValueItem as top level item
  handler->OnAddEmptyAnyValue();

  // validating that model got top level item of the correct type
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  auto inserted_item = mvvm::utils::GetTopItem<sup::gui::AnyValueEmptyItem>(&m_model);
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), ::sup::gui::constants::kEmptyTypeName);

  EXPECT_EQ(testutils::GetSendItem<mvvm::SessionItem>(spy_selection_request), inserted_item);
};

// -------------------------------------------------------------------------------------------------
// Adding structures
// -------------------------------------------------------------------------------------------------

//! Adding structure to an empty model.

TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueStructToEmptyModel)
{
  // creating action for the context, when nothing is selected by the user
  auto handler = CreateActionHandler(nullptr);

  QSignalSpy spy_selection_request(handler.get(), &AnyValueEditorActionHandler::SelectItemRequest);

  EXPECT_EQ(handler->GetSelectedItem(), nullptr);

  // expecting no warnings
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as top level item
  handler->OnAddAnyValueStruct();

  // validating that model got top level item of the correct type
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  auto inserted_item = mvvm::utils::GetTopItem<sup::gui::AnyValueStructItem>(&m_model);
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), ::sup::gui::constants::kStructTypeName);

  EXPECT_EQ(testutils::GetSendItem<mvvm::SessionItem>(spy_selection_request), inserted_item);
};

//! Attempt to add a structure to a non-empty model when nothing is selected.

TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddToNonEmptyModel)
{
  // non-empty model
  m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // creating action for the context, when nothing is selected by the user
  auto handler = CreateActionHandler(nullptr);

  // expecting warning
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // attempt to add another top level item
  handler->OnAddAnyValueStruct();

  // validating that there is still one item
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
};

//! Adding structure as a field to another structure (which is marked as selected).

TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueStructToAnotherStruct)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // creating action for the context, when parent is selected
  auto handler = CreateActionHandler(parent);
  EXPECT_EQ(handler->GetSelectedItem(), parent);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as a field
  handler->OnAddAnyValueStruct();

  // validating that parent got new child
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  EXPECT_EQ(inserted_item->GetType(), std::string("AnyValueStruct"));

  const std::string expected_field_name(kFieldNamePrefix + "0");
  EXPECT_EQ(inserted_item->GetDisplayName(), expected_field_name);
};

//! Attempt to add a structure as a field to a scalar.

TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddStructToScalar)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueScalarItem>();

  // creating action for the context, when parent is selected
  auto handler = CreateActionHandler(parent);

  // expecting error callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // adding AnyValueItem struct as a field to
  handler->OnAddAnyValueStruct();

  // validating that nothing can changed in the model
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 0);
};

//-------------------------------------------------------------------------------------------------
// Adding scalars
//-------------------------------------------------------------------------------------------------

//! Adding a scalar to an empty model.

TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueScalarToEmptyModel)
{
  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler(nullptr);

  // adding AnyValueItem struct as top level item
  handler->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // validating that model got top level item of the correct type
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  auto inserted_item = mvvm::utils::GetTopItem<sup::gui::AnyValueScalarItem>(&m_model);
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), ::sup::gui::constants::kScalarTypeName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);

  // expecting warning callback further down
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // adding another scalar when nothing is selected should trigger the warning
  handler->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // the amount of items should stay the same
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
};

//! Adding scalar as a field to another structure (which is marked as selected).

TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueScalarToStruct)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // creating action handler for the context, when parent is selected
  auto handler = CreateActionHandler(parent);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as a field
  handler->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // validating that parent got new child
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  const std::string expected_field_name(kFieldNamePrefix + "0");
  EXPECT_EQ(inserted_item->GetDisplayName(), expected_field_name);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->GetToolTip(), sup::dto::kInt32TypeName);
};

//! Adding a scalar as an array element (which is marked as selected).

TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueScalarToArray)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueArrayItem>();

  // creating action handler for the context, when parent is selected
  auto handler = CreateActionHandler(parent);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as a field
  handler->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // validating that parent got new child
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  const std::string expected_field_name(kElementNamePrefix + "0");
  EXPECT_EQ(inserted_item->GetDisplayName(), expected_field_name);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->GetToolTip(), sup::dto::kInt32TypeName);
};

//! Attempt to add scalar as a field to another scalar.

TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddScalarToScalar)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueScalarItem>();

  // creating action handler for the context, when parent is selected
  auto handler = CreateActionHandler(parent);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // adding AnyValueItem struct as a field
  handler->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // validating that nothing can changed in the model
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 0);
};

//! Attempt to add second top level scalar to the model.

TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddSecondTopLevelScalar)
{
  m_model.InsertItem<sup::gui::AnyValueScalarItem>();

  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler(nullptr);

  // expecting warning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // attempt to add second top level scalar
  handler->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // checking that model still have a single item
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
};

//! Attempt to add a scalar as an array element when array is contasining diffierent scalar types.

TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddScalarToArrayWhenTypeMismath)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueArrayItem>();
  m_model.InsertItem<sup::gui::AnyValueScalarItem>(parent)->SetAnyTypeName(
      sup::dto::kInt32TypeName);

  // creating action handler for the context, when parent is selected
  auto handler = CreateActionHandler(parent);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem scalar as a field. The type matches what is already in the array.
  handler->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // validating that parent got new child
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 2);

  // expecting error callback
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // attempt to add mismatching type
  handler->OnAddAnyValueScalar(sup::dto::kInt16TypeName);

  // array still has two element
  EXPECT_EQ(parent->GetChildren().size(), 2);
};

//-------------------------------------------------------------------------------------------------
// Adding array
//-------------------------------------------------------------------------------------------------

//! Adding a scalar to an empty model.

TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueArrayToEmptyModel)
{
  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler(nullptr);

  // adding AnyValueItem struct as top level item
  handler->OnAddAnyValueArray();

  // validating that model got top level item of the correct type
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  auto inserted_item = mvvm::utils::GetTopItem<sup::gui::AnyValueArrayItem>(&m_model);
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), ::sup::gui::constants::kArrayTypeName);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // attempt to add second top-level item
  handler->OnAddAnyValueArray();

  // the amount of items should stay the same
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
};

//! Adding array as a field to another structure (which is marked as selected).

TEST_F(AnyValueEditorActionHandlerTest, OnAddAnyValueArrayToStruct)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // creating action handler for the context, when parent is selected
  auto handler = CreateActionHandler(parent);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as a field
  handler->OnAddAnyValueArray();

  // validating that parent got new child
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  EXPECT_EQ(inserted_item->GetType(), std::string("AnyValueArray"));
  const std::string expected_field_name(kFieldNamePrefix + "0");
  EXPECT_EQ(inserted_item->GetDisplayName(), expected_field_name);
};

//! Attempt to add array as a field to a scalar.

TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddArrayToScalar)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueScalarItem>();

  // creating action handler for the context, when parent is selected
  auto handler = CreateActionHandler(parent);

  // expecting error callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // adding AnyValueItem struct as a field to
  handler->OnAddAnyValueArray();

  // validating that nothing can changed in the model
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 0);
};

//! Attempt to add second top level array to the model.

TEST_F(AnyValueEditorActionHandlerTest, AttemptToAddSecondTopLevelArray)
{
  m_model.InsertItem<sup::gui::AnyValueArrayItem>();

  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler(nullptr);

  // expecting warning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // attempt to add second top level array
  handler->OnAddAnyValueArray();

  // checking that model still have a single item
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
};

//-------------------------------------------------------------------------------------------------
// Remove item
//-------------------------------------------------------------------------------------------------

//! Remove item when nothing is selected.

TEST_F(AnyValueEditorActionHandlerTest, RemoveItemWhenNothingIsSelected)
{
  auto struct_item = m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler(nullptr);

  EXPECT_NO_FATAL_FAILURE(handler->OnRemoveSelected());

  // validating that still has an item
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
};

//! Remove selected item.

TEST_F(AnyValueEditorActionHandlerTest, RemoveSelectedItem)
{
  auto struct_item = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);

  // creating action handler for the context, pretending item is selected
  auto handler = CreateActionHandler(struct_item);

  handler->OnRemoveSelected();

  // validating that there is no item anymore
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 0);
};

//-------------------------------------------------------------------------------------------------
// Import/export
//-------------------------------------------------------------------------------------------------

//! Validates import of JSON from file.

TEST_F(AnyValueEditorActionHandlerTest, ImportFromFile)
{
  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalar.xml");
  sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type, 42};
  auto json_content = AnyValueToJSONString(anyvalue);
  testutils::CreateTextFile(file_path, json_content);

  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler(nullptr);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  handler->OnImportFromFileRequest(file_path);

  // validating that model got top level item of the correct type
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  auto inserted_item = mvvm::utils::GetTopItem<sup::gui::AnyValueScalarItem>(&m_model);
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), sup::gui::constants::kScalarTypeName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->Data<int>(), 42);

  // attempt to import again
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  handler->OnImportFromFileRequest(file_path);
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
};

//! Validates import of JSON from file, where imported value goes as a field to selected
//! structure.

TEST_F(AnyValueEditorActionHandlerTest, ImportFromFileToStructField)
{
  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalar.xml");
  sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type, 42};
  auto json_content = AnyValueToJSONString(anyvalue);
  testutils::CreateTextFile(file_path, json_content);

  auto structure = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);

  // creating action handler for the context, making structure selected
  auto handler = CreateActionHandler(structure);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  handler->OnImportFromFileRequest(file_path);

  // testing new child of the structure
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(structure->GetChildren().size(), 1);
  auto inserted_item = dynamic_cast<sup::gui::AnyValueScalarItem*>(structure->GetChildren().at(0));
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), sup::gui::constants::kScalarTypeName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->Data<int>(), 42);
};

//! Validates export of top level item to JSON file.

TEST_F(AnyValueEditorActionHandlerTest, ExportToFile)
{
  // preparing scalar
  auto scalar = m_model.InsertItem<sup::gui::AnyValueScalarItem>();
  scalar->SetAnyTypeName(sup::dto::kInt32TypeName);
  scalar->SetData(99);

  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalarExportResults.xml");

  // creating action handler when nothing is selected
  auto handler = CreateActionHandler(nullptr);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // exporting file
  handler->OnExportToFileRequest(file_path);

  // model should be the same
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 1);

  // reading our exported file for the validation
  auto exported_value = sup::gui::AnyValueFromJSONFile(file_path);

  sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 99};
  EXPECT_EQ(exported_value, expected_anyvalue);
};

//! Attempt to export to JSON file from epmpty model.

TEST_F(AnyValueEditorActionHandlerTest, AttemptToExportEmptyModelToFile)
{
  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalarExportResultsV2.xml");

  // creating action when nothing is selected
  auto actions = CreateActionHandler(nullptr);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // exporting file
  actions->OnExportToFileRequest(file_path);

  // model empty as it was, file wasn't created
  EXPECT_EQ(GetAnyValueItemContainer()->GetTotalItemCount(), 0);
  EXPECT_FALSE(mvvm::utils::IsExists(file_path));
};

//-------------------------------------------------------------------------------------------------
// Move Up/Down
//-------------------------------------------------------------------------------------------------

TEST_F(AnyValueEditorActionHandlerTest, MoveUp)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  auto field0 = parent->AddScalarField("field0", sup::dto::kInt32TypeName, mvvm::int32{42});
  auto field1 = parent->AddScalarField("field1", sup::dto::kInt32TypeName, mvvm::int32{43});

  // creating action handler for the context, when field1 is selected
  auto handler = CreateActionHandler(field1);

  QSignalSpy spy_selection_request(handler.get(), &AnyValueEditorActionHandler::SelectItemRequest);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // moving selected item up
  handler->OnMoveUpRequest();

  // validating that parent got new child
  EXPECT_EQ(parent->GetChildren(), std::vector<sup::gui::AnyValueItem*>({field1, field0}));

  EXPECT_EQ(testutils::GetSendItem<mvvm::SessionItem>(spy_selection_request), field1);

  // moving selected item up second time doesn't change anything
  handler->OnMoveUpRequest();

  EXPECT_EQ(parent->GetChildren(), std::vector<sup::gui::AnyValueItem*>({field1, field0}));
  EXPECT_EQ(spy_selection_request.count(), 0);
};
