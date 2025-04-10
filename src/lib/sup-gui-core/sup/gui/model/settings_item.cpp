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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "settings_item.h"

#include "settings_constants.h"

#include <mvvm/signals/item_connect.h>

namespace sup::gui
{

CommonSettingsItem::CommonSettingsItem() : CompoundItem(GetStaticType())
{
  (void)SetDisplayName("Common Settings");

  (void)AddProperty(constants::kUseUndoSetting, constants::kUseUndoDefault)
      .SetDisplayName("Enable undo/redo");
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

void CommonSettingsItem::Activate()
{
  // Enable/disable property "Undo limit" when property "Enable undo/redo" changes
  auto on_property_changed = [this](const mvvm::DataChangedEvent&)
  {
    GetItem(constants::kUndoLimitSetting)->SetEnabled(Property<bool>(constants::kUseUndoSetting));
  };

  mvvm::connect::Connect<mvvm::DataChangedEvent>(
      /*source*/ GetItem(constants::kUseUndoSetting), on_property_changed, GetSlot());
}

}  // namespace sup::gui
