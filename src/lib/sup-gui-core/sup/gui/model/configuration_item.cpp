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

#include "configuration_item.h"

#include "anyvalue_item.h"
#include "anyvalue_item_constants.h"
#include "anyvalue_item_utils.h"

#include <mvvm/model/make_clone.h>

namespace sup::gui
{

namespace
{
constexpr auto kFileName = "kFileName";
}

ConfigurationItem::ConfigurationItem()
    : CompoundItem(mvvm::GetTypeName<ConfigurationItem>())
{
  (void)AddProperty(kFileName, std::string())
      .SetDisplayName("File name")
      .SetToolTip("The name of the file from which a configuration was imported");

  // tag for AnyValueItem, can hold one maximum
  RegisterTag(mvvm::TagInfo(constants::kAnyValueChildrenTag, 0, 1, GetAnyValueItemTypes()),
              /*as_default*/ true);

  (void)SetFlag(mvvm::Appearance::kEditableDisplayName, true);
}

std::unique_ptr<mvvm::SessionItem> ConfigurationItem::Clone() const
{
  return mvvm::MakeClone(*this);
}

AnyValueItem* ConfigurationItem::GetValue()
{
  return GetItem<AnyValueItem>(constants::kAnyValueChildrenTag);
}

std::string ConfigurationItem::GetFileName() const
{
  return Property<std::string>(kFileName);
}

void ConfigurationItem::SetFileName(const std::string& value)
{
  (void)SetProperty(kFileName, value);
}

}  // namespace sup::gui
