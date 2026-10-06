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

#include "register_items.h"

#include "anyvalue_item.h"
#include "common_settings_item.h"
#include "scalartype_property_item.h"
#include "settings_container_item.h"

#include <mvvm/model/item_factory.h>

namespace sup::gui
{

void RegisterSessionItems()
{
  (void)mvvm::RegisterGlobalItem<AnyValueArrayItem>();
  (void)mvvm::RegisterGlobalItem<AnyValueEmptyItem>();
  (void)mvvm::RegisterGlobalItem<AnyValueScalarItem>();
  (void)mvvm::RegisterGlobalItem<AnyValueStructItem>();
  (void)mvvm::RegisterGlobalItem<CommonSettingsItem>();
  (void)mvvm::RegisterGlobalItem<ScalarTypePropertyItem>();
  (void)mvvm::RegisterGlobalItem<SettingsContainerItem>();
}

}  // namespace sup::gui
