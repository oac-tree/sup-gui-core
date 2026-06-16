/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "sup/gui/views/dtoeditor/dto_explorer_view.h"

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>
#include <sup/gui/model/anyvalue_utils.h>
#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/views/codeeditor/code_view.h>
#include <sup/gui/views/dtoeditor/dto_explorer_panel.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>
#include <testutils/test_helper.h>

namespace sup::gui::test
{

/**
 * @brief Tests for DtoExplorerView class.
 */
class DtoExplorerViewTest : public test::FolderTest
{
public:
  DtoExplorerViewTest() : test::FolderTest("DtoExplorerViewTest") {}
};

TEST_F(DtoExplorerViewTest, ImportFileOnDoubleClick)
{
  // preparing file with content for further import
  const auto file_path = GetFilePath("ImportFromFile.json");
  const sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type, 42};
  test::CreateSerializedFile(anyvalue, file_path);

  DtoExplorerView explorer_view;
  explorer_view.show();
  ConfigurationModel model;
  explorer_view.SetConfigurationModel(&model);

  // explorer panel asks to import file
  auto explorer_panel = explorer_view.findChild<DtoExplorerPanel*>();
  ASSERT_NE(explorer_panel, nullptr);
  explorer_panel->FileDoubleClicked(QString::fromStdString(file_path));

  // the model should have new configuration item
  auto container = model.GetConfigurationContainer();
  ASSERT_EQ(container->GetConfigurationCount(), 1);

  auto anyvalue_item = container->GetConfigurations().at(0)->GetValue();
  auto stored_anyvalue = CreateAnyValue(*anyvalue_item);
  EXPECT_EQ(stored_anyvalue, anyvalue);
}

TEST_F(DtoExplorerViewTest, ShowTextOnSingleClick)
{
  const auto file_path = GetFilePath("ImportFromFile.json");
  const sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type, 42};
  test::CreateSerializedFile(anyvalue, file_path);

  DtoExplorerView explorer_view;
  explorer_view.show();

  auto code_view = explorer_view.findChild<CodeView*>();
  ASSERT_NE(code_view, nullptr);

  EXPECT_EQ(code_view->GetContent(), "");

  // explorer panel asks to show file content
  auto explorer_panel = explorer_view.findChild<DtoExplorerPanel*>();
  ASSERT_NE(explorer_panel, nullptr);

  explorer_panel->FileClicked(QString::fromStdString(file_path));

  auto expected_json_content = AnyValueToJSONString(anyvalue) + "\n";
  EXPECT_EQ(code_view->GetContent(), QString::fromStdString(expected_json_content));
}

}  // namespace sup::gui::test
