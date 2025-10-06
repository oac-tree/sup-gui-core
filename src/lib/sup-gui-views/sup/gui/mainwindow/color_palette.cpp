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

#include <mvvm/style/mvvm_style_helper.h>

#include <QAnyStringView>

namespace sup::gui
{

namespace
{

constexpr QAnyStringView kGenericViridian = "#008a64";
constexpr QAnyStringView kMediumJungleGreen = "#1A3636";

}  // namespace

QColor GetMainToolBarColor()
{
  return mvvm::style::IsDarkTheme() ? QColor::fromString(kMediumJungleGreen)
                                    : QColor::fromString(kGenericViridian);
}

}  // namespace sup::gui
