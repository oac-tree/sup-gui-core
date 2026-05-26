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

#include "sup/gui/model/import_helper.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/test/test_helper.h>

#include <sup/dto/anyvalue.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/folder_test.h>

namespace sup::gui::test
{

/**
 * @brief Tests for helper methods from import_helper.h.
 */
class ImportHelperTest : public test::FolderTest
{
public:
  ImportHelperTest() : test::FolderTest("ImportHelperTest") {}

  ::testing::MockFunction<void(const MessageEvent&)> m_error_callback;
};

TEST_F(ImportHelperTest, ImportFromFile)
{
  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalar.xml");
  const sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type, 42};
  auto json_content = AnyValueToJSONString(anyvalue);
  mvvm::test::CreateTextFile(file_path, json_content);

  EXPECT_CALL(m_error_callback, Call(::testing::_)).Times(0);

  // importing item from file
  auto item = CreateAnyValueItemFromFile(file_path, m_error_callback.AsStdFunction());

  // validating that item got correct type and value
  ASSERT_NE(item, nullptr);
  EXPECT_EQ(item->GetDisplayName(), constants::kScalarTypeName);
  EXPECT_EQ(item->GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(item->Data<int>(), 42);
}

TEST_F(ImportHelperTest, ImportFromNonExistingFile)
{
  EXPECT_CALL(m_error_callback, Call(::testing::_)).Times(0);
  auto item = CreateAnyValueItemFromFile("non-existinf-file", m_error_callback.AsStdFunction());
}

}  // namespace sup::gui::test
