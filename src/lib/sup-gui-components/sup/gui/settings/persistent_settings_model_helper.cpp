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

#include "persistent_settings_model_helper.h"

#include <sup/gui/model/persistent_settings_model.h>
#include <sup/gui/model/undo_settings_item.h>
#include <sup/gui/settings/i_settings_manager.h>

#include <mvvm/model/model_utils.h>

namespace sup::gui
{

std::unique_ptr<PersistentSettingsModel> CreatePersistentSettingsModel(
    const ISettingsManager& manager)
{
  auto result = std::make_unique<PersistentSettingsModel>();

  mvvm::utils::GetTopItem<UndoSettingsItem>(*result)->SetUndoInfo(
      manager.Get<UndoInfo>(kUndoSettingsGroup, SettingsScope::kApplication));

  return result;
}

void WritePersistentSettingsModel(const PersistentSettingsModel& model, ISettingsManager& manager)
{
  manager.Set(kUndoSettingsGroup, mvvm::utils::GetTopItem<UndoSettingsItem>(model)->GetUndoInfo(),
              SettingsScope::kApplication);
}

}  // namespace sup::gui
