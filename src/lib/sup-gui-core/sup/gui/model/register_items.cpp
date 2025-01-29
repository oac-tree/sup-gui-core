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

#include "register_items.h"

#include "anyvalue_item.h"

#include <mvvm/model/item_factory.h>

namespace sup::gui
{

void RegisterSessionItems()
{
  (void)mvvm::RegisterGlobalItem<sup::gui::AnyValueEmptyItem>();
  (void)mvvm::RegisterGlobalItem<sup::gui::AnyValueScalarItem>();
  (void)mvvm::RegisterGlobalItem<sup::gui::AnyValueStructItem>();
  (void)mvvm::RegisterGlobalItem<sup::gui::AnyValueArrayItem>();
}

}  // namespace sup::gui
