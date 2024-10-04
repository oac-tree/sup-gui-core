/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "custom_metatypes.h"

#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/line_series_item.h>

namespace sup::gui
{

void RegisterCustomMetaTypes()
{
  qRegisterMetaType<mvvm::SessionItem*>("mvvm::SessionItem*");
  qRegisterMetaType<mvvm::LineSeriesItem*>("mvvm::LineSeriesItem*");
}

}  // namespace sup::gui
