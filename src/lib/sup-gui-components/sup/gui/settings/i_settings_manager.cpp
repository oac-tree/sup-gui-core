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

#include "i_settings_manager.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

namespace sup::gui
{

namespace
{

//! Returns the name of the scope for error messages.
std::string GetScopeName(SettingsScope scope)
{
  return (scope == SettingsScope::kApplication) ? "application" : "project";
}

}  // namespace

ISettingsStorage& ISettingsManager::GetValidStorage(SettingsScope scope) const
{
  auto storage = GetStorage(scope);
  if (storage == nullptr)
  {
    throw RuntimeException("Settings storage is not set for scope [" + GetScopeName(scope) + "]");
  }
  return *storage;
}

}  // namespace sup::gui
