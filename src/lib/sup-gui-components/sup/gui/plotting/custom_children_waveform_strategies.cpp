/******************************************************************************
 *
 * Project       : PSPS editor
 *
 * Description   : Graphical user interface for pulse schedule preparation system
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

#include "custom_children_waveform_strategies.h"

#include <mvvm/standarditems/line_series_data_item.h>

namespace sup::gui
{

std::vector<mvvm::SessionItem *> WaveformChildrenStrategy::GetChildren(
    const mvvm::SessionItem *item) const
{
  // The strategy reports LineSeriesDataItem's children (which are structs carraying "x" and "y") as
  // it is.
  if (auto data_item = dynamic_cast<const mvvm::LineSeriesDataItem *>(item); data_item)
  {
    return data_item->GetAllItems();
  }

  // For all others pretend no children.
  return {};
}

}  // namespace sup::gui
