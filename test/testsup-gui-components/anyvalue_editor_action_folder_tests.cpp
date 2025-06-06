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
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/test/test_helper.h>
#include <mvvm/utils/file_utils.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>
#include <testutils/mock_anyvalue_editor_context.h>

#include <QMimeData>

namespace sup::gui::test
{

/**
 * @brief Tests for AnyValueEditorActionHandlerFolderTest class for import/export scenario.
 */
class AnyValueEditorActionHandlerFolderTest : public test::FolderTest
{
public:
  AnyValueEditorActionHandlerFolderTest()
      : test::FolderTest("AnyValueEditorActionHandlerFolderTest")
  {
  }

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
  mvvm::SessionItem* GetContainer() { return m_model.GetRootItem(); }

  mvvm::ApplicationModel m_model;
  test::MockAnyValueEditorContext m_mock_context;
};

//! Validates import of JSON from file.
TEST_F(AnyValueEditorActionHandlerFolderTest, ImportFromFile)
{
  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalar.xml");
  const sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type, 42};
  auto json_content = AnyValueToJSONString(anyvalue);
  mvvm::test::CreateTextFile(file_path, json_content);

  // creating action handler for the context, when nothing is selected by the user
  auto handler = CreateActionHandler({});

  // expecting no callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);

  handler->OnImportFromFileRequest(file_path);

  // validating that model got top level item of the correct type
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  auto inserted_item = mvvm::utils::GetTopItem<sup::gui::AnyValueScalarItem>(&m_model);
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), sup::gui::constants::kScalarTypeName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->Data<int>(), 42);

  // attempt to import again
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  handler->OnImportFromFileRequest(file_path);
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
};

//! Validates import of JSON from file, where imported value goes as a field to selected
//! structure.
TEST_F(AnyValueEditorActionHandlerFolderTest, ImportFromFileToStructField)
{
  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalar.xml");
  const sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type, 42};
  auto json_content = AnyValueToJSONString(anyvalue);
  mvvm::test::CreateTextFile(file_path, json_content);

  auto structure = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);

  // creating action handler for the context, making structure selected
  auto handler = CreateActionHandler({structure});

  // expecting no callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);

  handler->OnImportFromFileRequest(file_path);

  // testing new child of the structure
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);
  ASSERT_EQ(structure->GetChildren().size(), 1);
  auto inserted_item = dynamic_cast<sup::gui::AnyValueScalarItem*>(structure->GetChildren().at(0));
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), sup::gui::constants::kScalarTypeName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->Data<int>(), 42);
};

//! Validates import of JSON from file, on attempt to import into a scalar.
TEST_F(AnyValueEditorActionHandlerFolderTest, ImportFromFileToScalar)
{
  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalar.xml");
  const sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type, 42};
  auto json_content = AnyValueToJSONString(anyvalue);
  mvvm::test::CreateTextFile(file_path, json_content);

  auto scalar_item = m_model.InsertItem<sup::gui::AnyValueScalarItem>();

  // creating action handler for the context, making structure selected
  auto handler = CreateActionHandler({scalar_item});

  // expecting error callbacks
  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  handler->OnImportFromFileRequest(file_path);
};

//! Validates export of top level item to JSON file.
TEST_F(AnyValueEditorActionHandlerFolderTest, ExportToFile)
{
  // preparing scalar
  auto scalar = m_model.InsertItem<sup::gui::AnyValueScalarItem>();
  scalar->SetAnyTypeName(sup::dto::kInt32TypeName);
  scalar->SetData(99);

  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalarExportResults.xml");

  // creating action handler when nothing is selected
  auto handler = CreateActionHandler({});

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(0);

  // exporting file
  handler->OnExportToFileRequest(file_path);

  // model should be the same
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 1);

  // reading our exported file for the validation
  auto exported_value = sup::gui::AnyValueFromJSONFile(file_path);

  const sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 99};
  EXPECT_EQ(exported_value, expected_anyvalue);
};

//! Attempt to export to JSON file from empty model.
TEST_F(AnyValueEditorActionHandlerFolderTest, AttemptToExportEmptyModelToFile)
{
  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalarExportResultsV2.xml");

  // creating action when nothing is selected
  auto actions = CreateActionHandler({});

  EXPECT_CALL(m_mock_context, OnMessage(::testing::_)).Times(1);

  // exporting file
  actions->OnExportToFileRequest(file_path);

  // model empty as it was, file wasn't created
  EXPECT_EQ(GetContainer()->GetTotalItemCount(), 0);
  EXPECT_FALSE(mvvm::utils::IsExists(file_path));
};

}  // namespace sup::gui::test
