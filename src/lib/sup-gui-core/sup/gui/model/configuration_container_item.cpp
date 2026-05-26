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

#include "anyvalue_item.h"
#include "anyvalue_item_constants.h"
#include "anyvalue_item_utils.h"

namespace sup::gui
{

namespace
{
constexpr auto kFileName = "kFileName";
}

ConfigurationContainerItem::ConfigurationContainerItem()
    : CompoundItem(mvvm::GetTypeName<ConfigurationContainerItem>())
{
  (void)AddProperty(kFileName, std::string())
      .SetDisplayName("File name")
      .SetToolTip("The name of the file from which a procedure was imported");

  // tag for AnyValueItem, can hold one maximum
  RegisterTag(mvvm::TagInfo(constants::kAnyValueChildrenTag, 0, 1, GetAnyValueItemTypes()),
              /*as_default*/ true);
}

AnyValueItem* ConfigurationContainerItem::GetConfiguration()
{
  return GetItem<AnyValueItem>(constants::kAnyValueChildrenTag);
}

std::unique_ptr<mvvm::SessionItem> ConfigurationContainerItem::Clone() const
{
  return std::make_unique<ConfigurationContainerItem>(*this);
}

std::string ConfigurationContainerItem::GetFileName() const
{
  return Property<std::string>(kFileName);
}

void ConfigurationContainerItem::SetFileName(const std::string& value)
{
  (void)SetProperty(kFileName, value);
}

}  // namespace sup::gui
