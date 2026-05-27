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

#include "configuration_container_item.h"

#include "configuration_item.h"

namespace sup::gui
{

ConfigurationContainerItem::ConfigurationContainerItem()
    : CompoundItem(mvvm::GetTypeName<ConfigurationContainerItem>())
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kChildrenTag),
              /*as_default*/ true);
}

std::unique_ptr<mvvm::SessionItem> ConfigurationContainerItem::Clone() const
{
  return std::make_unique<ConfigurationContainerItem>(*this);
}

std::vector<ConfigurationItem*> ConfigurationContainerItem::GetConfigurations()
{
  return GetItems<ConfigurationItem>(kChildrenTag);
}

std::size_t ConfigurationContainerItem::GetConfigurationCount() const
{
  return GetItemCount(kChildrenTag);
}

}  // namespace sup::gui
