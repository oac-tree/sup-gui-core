/******************************************************************************
 *
 * Project       : PSPS editor
 *
 * Description   : Graphical user interface for pulse schedule preparation system
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "custom_children_waveform_strategies.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/session_item.h>

namespace sup::gui
{

std::vector<mvvm::SessionItem *> WaveformChildrenStrategy::GetChildren(
    const mvvm::SessionItem *item) const
{
  // The strategy reports AnyValueArrayItem's children (which are structs carraying "x" and "y") as
  // it is.
  if (auto anyvalue_item = dynamic_cast<const sup::gui::AnyValueItem *>(item); anyvalue_item)
  {
    if (anyvalue_item->IsArray())
    {
      return anyvalue_item->GetAllItems();
    }
  }

  // For all others pretend no children.
  return {};
}

}  // namespace sup::gui
