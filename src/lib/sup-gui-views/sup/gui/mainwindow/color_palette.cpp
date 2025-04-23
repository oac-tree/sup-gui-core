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

#include "color_palette.h"

#include <sup/gui/style/style_helper.h>

namespace sup::gui
{

namespace
{

const char *kGenericViridian = "#008a64";
const char *kMediumJungleGreen = "#1A3636";

}  // namespace

QColor GetMainToolBarColor()
{
  return sup::gui::utils::IsDarkTheme() ? QColor(kMediumJungleGreen) : QColor(kGenericViridian);
}

}  // namespace sup::gui
