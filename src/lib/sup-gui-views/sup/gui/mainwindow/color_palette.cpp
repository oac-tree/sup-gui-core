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

#include "color_palette.h"

#include <sup/gui/widgets/style_utils.h>

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
