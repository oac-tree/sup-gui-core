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

#ifndef SUP_GUI_STYLE_ICON_COLOR_FLAVOR_H_
#define SUP_GUI_STYLE_ICON_COLOR_FLAVOR_H_

#include <cstdint>

namespace sup::gui
{

/**
 * @brief The IconColorFlavor enum defines the dark/light type of the icon.
 */
enum class IconColorFlavor : std::uint8_t
{
  kUnspecified,    //!< dark or light will be automatically deduced from current theme
  kForDarkThemes,  //!< icon is intended for dark background
  kForLightThemes  //!< icon is intended for light background
};

}  // namespace sup::gui

#endif  // SUP_GUI_STYLE_ICON_COLOR_FLAVOR_H_
