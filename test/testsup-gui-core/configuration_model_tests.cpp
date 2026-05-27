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

#include "sup/gui/model/configuration_model.h"

#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for helper methods from import_helper.h.
 */
class ConfigurationModelTest : public ::testing::Test
{
public:
};

TEST_F(ConfigurationModelTest, InitialState)
{
  ConfigurationModel model;
  ASSERT_NE(model.GetConfigurationContainer(), nullptr);
  EXPECT_EQ(model.GetConfigurationContainer()->GetConfigurationCount(), 0U);
}

TEST_F(ConfigurationModelTest, AddConfiguration)
{
  ConfigurationModel model;
  ASSERT_NE(model.GetConfigurationContainer(), nullptr);
  EXPECT_EQ(model.GetConfigurationContainer()->GetConfigurationCount(), 0U);

  // clear doesn't remove container
  model.Clear();
  ASSERT_NE(model.GetConfigurationContainer(), nullptr);
  EXPECT_EQ(model.GetConfigurationContainer()->GetConfigurationCount(), 0U);

  auto config = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                    mvvm::TagIndex::Append());

  EXPECT_EQ(model.GetConfigurationContainer()->GetConfigurationCount(), 1U);

  // clear removes configuration
  model.Clear();
  ASSERT_NE(model.GetConfigurationContainer(), nullptr);
  EXPECT_EQ(model.GetConfigurationContainer()->GetConfigurationCount(), 0U);
}

}  // namespace sup::gui::test
