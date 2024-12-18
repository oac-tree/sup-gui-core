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

#ifndef SUP_GUI_STYLE_STYLE_HELPER_H_
#define SUP_GUI_STYLE_STYLE_HELPER_H_

#include <sup/gui/style/icon_color_flavor.h>

#include <QWidget>

class QIcon;
class QTreeView;
class QIconEngine;

namespace sup::gui::utils
{

/**
 * @brief Checks if main application theme is dark.
 */
bool IsDarkTheme();

/**
 * @brief Returns icon color corresponding to a given icon flavor.
 */
QColor GetIconBaseColor(IconColorFlavor icon_flavor);

/**
 * @brief Returns an icon for a given resource name and dark/light color flavor.
 *
 * Works only for svg resources.
 * Example
 * GetIcon(:/sup-gui-core/icons/animation.svg", AppIconColorFlavor::kForDarkThemes)
 *
 * @param Full path to svg icon as defined in application resource file.
 * @param icon_type The desired dark/light type of the icon.
 *
 * @note We use flat-style monochrome SVG icons. When icon_type is unspecified, icons will be
 * rendered according to the current theme's dark/light flavor. Icons for the dark theme will be
 * rendered as almost white, icons for the light theme will be rendered as almost black. User can
 * still specify the desired icon_flavor thus overriding the current theme's dark/light style.
 */
QIcon GetIcon(const QString& resource_name,
              IconColorFlavor icon_flavor = IconColorFlavor::kUnspecified);

/**
 * @brief Finds an icon with the given name and dark/light color flavor.
 *
 * Example: FindIcon("animation", AppIconColorFlavor::kForDarkThemes)
 * Is assumes an existence of animation.svg file in the resource folder of sup-gui-core package.
 */
QIcon FindIcon(const QString& icon_name,
               IconColorFlavor icon_flavor = IconColorFlavor::kUnspecified);

/**
 * @brief Size of main tolbar icons.
 */
QSize ToolBarIconSize();

/**
 * @brief Size of narrow tolbar icons.
 */
QSize NarrowToolBarIconSize();

/**
 * @brief Sest visual style of the tree to the "breeze"-like style, if necessary.
 *
 * On CODAC system's the default desktop style is "fusion". The tree has a flat look,  without
 * appealing lines connecting expand/collapse markers. Call to this method will try to fix this.
 */
void BeautifyTreeStyle(QTreeView* tree);

}  // namespace sup::gui::utils

#endif  // SUP_GUI_STYLE_STYLE_HELPER_H_
