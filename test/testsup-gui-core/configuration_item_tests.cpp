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

#include "sup/gui/model/configuration_item.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/core/mvvm_exceptions.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

class ConfigurationItemTest : public ::testing::Test
{
};

TEST_F(ConfigurationItemTest, InitialSate)
{
  ConfigurationItem configuration;

  EXPECT_TRUE(configuration.GetFileName().empty());
  EXPECT_EQ(configuration.GetConfiguration(), nullptr);
}

TEST_F(ConfigurationItemTest, GetSetFileName)
{
  ConfigurationItem configuration;

  configuration.SetFileName("abc");
  EXPECT_EQ(configuration.GetFileName(), std::string("abc"));
}

TEST_F(ConfigurationItemTest, InsertAnyValueItem)
{
  ConfigurationItem configuration;

  auto anyvalue_item = configuration.InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  EXPECT_NE(anyvalue_item, nullptr);
  EXPECT_EQ(configuration.GetConfiguration(), anyvalue_item);

  // you can't insert second item
  EXPECT_THROW(configuration.InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append()),
               mvvm::InvalidOperationException);
}

}  // namespace sup::gui::test
