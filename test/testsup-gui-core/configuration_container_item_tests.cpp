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

#include "sup/gui/model/configuration_container_item.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/configuration_item.h>

#include <mvvm/core/mvvm_exceptions.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

class ConfigurationContainerItemTest : public ::testing::Test
{
};

TEST_F(ConfigurationContainerItemTest, InitialSate)
{
  ConfigurationContainerItem container;

  EXPECT_TRUE(container.GetConfigurations().empty());
  EXPECT_EQ(container.GetConfigurationCount(), 0U);
}

TEST_F(ConfigurationContainerItemTest, InsertConfiguration)
{
  ConfigurationContainerItem container;

  auto config0 = container.InsertItem<ConfigurationItem>(mvvm::TagIndex::Append());
  auto config1 = container.InsertItem<ConfigurationItem>(mvvm::TagIndex::Append());
  EXPECT_EQ(container.GetConfigurations(),
            std::vector<ConfigurationItem*>({config0, config1}));
  EXPECT_EQ(container.GetConfigurationCount(), 2U);
}

}  // namespace sup::gui::test
