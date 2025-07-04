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

#include "settings_item.h"

#include "settings_constants.h"

#include <mvvm/model/item_utils.h>

namespace sup::gui
{

CommonSettingsItem::CommonSettingsItem() : CompoundItem(GetStaticType())
{
  (void)SetDisplayName("Common Settings");

  AddProperty(constants::kUseUndoSetting, constants::kUseUndoDefault)
      .SetDisplayName("Enable undo/redo")
      .SetStrategy(this, &CommonSettingsItem::OnSetFlag);
  (void)AddProperty(constants::kUndoLimitSetting, constants::kUndoLimitDefault)
      .SetDisplayName("Undo limit");
}

std::string CommonSettingsItem::GetStaticType()
{
  return "CommonSettings";
}

std::unique_ptr<mvvm::SessionItem> CommonSettingsItem::Clone() const
{
  return std::make_unique<CommonSettingsItem>(*this);
}

bool CommonSettingsItem::OnSetFlag(SessionItem *property, const mvvm::variant_t &variant,
                                   mvvm::role_t role)
{
  mvvm::utils::BeginMacro(*this, "SetAvailableFlag");

  // propagate value to "undo flag" itself
  const bool result = mvvm::utils::SetData(*property, variant, role);

  // enable/disable property depending on undo flag value
  GetItem(constants::kUndoLimitSetting)->SetEnabled(std::get<bool>(variant));

  mvvm::utils::EndMacro(*this);

  return result;
}

}  // namespace sup::gui
