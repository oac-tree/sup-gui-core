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

#include "sup/gui/model/anyvalue_item.h"

#include <sup/gui/model/scalartype_property_item.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/application_model.h>

#include <sup/dto/anytype.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for AnyValueItem with the focus on undo/redo.
 */
class AnyValueItemTest : public ::testing::Test
{
};

TEST_F(AnyValueItemTest, ChangeScalarType)
{
  mvvm::ApplicationModel model;

  auto scalar = model.InsertItem<AnyValueScalarItem>();
  scalar->SetAnyTypeName(sup::dto::kInt8TypeName);
  scalar->SetData(mvvm::int8{42});
  EXPECT_EQ(scalar->GetAnyTypeName(), sup::dto::kInt8TypeName);
  EXPECT_EQ(scalar->Data<mvvm::int8>(), mvvm::int8{42});

  model.SetUndoEnabled(true);
  auto commands = model.GetCommandStack();
  EXPECT_FALSE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetCommandCount(), 0U);

  scalar->SetAnyTypeName(sup::dto::kStringTypeName);
  EXPECT_EQ(scalar->Data<std::string>(), std::string());

  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetCommandCount(), 1U);

  commands->Undo();
  // macro command was able to return everything in one go
  EXPECT_EQ(scalar->GetAnyTypeName(), sup::dto::kInt8TypeName);
  EXPECT_EQ(scalar->Data<mvvm::int8>(), mvvm::int8{42});

  commands->Redo();
  scalar->SetAnyTypeName(sup::dto::kStringTypeName);
  EXPECT_EQ(scalar->Data<std::string>(), std::string());
}

}  // namespace sup::gui::test
