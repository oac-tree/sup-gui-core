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

#ifndef SUP_GUI_WIDGETS_STYLE_UTILS_H_
#define SUP_GUI_WIDGETS_STYLE_UTILS_H_

#include <QWidget>

class QIcon;
class QTreeView;

namespace sup::gui::utils
{

/**
 * @brief The AppIconColorFlavor enum defines the dark/light type of the icon.
 */
enum AppIconColorFlavor
{
  kUnspecified,    //!< dark or light will be automatically deduced from current theme
  kForDarkThemes,  //!< icon is intended for dark themes
  kForLightThemes  //!< icon is intended for light themes
};

/**
 * @brief Size of main tolbar icons.
 */
QSize ToolBarIconSize();

/**
 * @brief Size of narrow tolbar icons.
 */
QSize NarrowToolBarIconSize();

/**
 * @brief Returns an icon for a given icon name.
 *
 * If the name doesn't contain an extension, will assume an existence of svg in the resource folder.

 * @param icon_type The desired dark/light type of the icon.
 *
 * @note Svg icons are special. We use flat-style colorless icons. When icon_type is
 * unspecified, icons will be rendered according to the current theme's dark/light flavor. Icons
 * for the dark theme will be rendered as almost white, icons for the light theme will be rendered
 * as almost black. User can still specify the desired icon_flavor thus overriding the current
 * theme's dark/light style.
 */
QIcon GetIcon(const QString& icon_name,
              AppIconColorFlavor icon_flavor = AppIconColorFlavor::kUnspecified);

/**
 * @brief Sest visual style of the tree to the "breeze"-like style, if necessary.
 *
 * On CODAC system's the default desktop style is "fusion". The tree has a flat look,  without
 * appealing lines connecting expand/collapse markers. Call to this method will try to fix this.
 */
void BeautifyTreeStyle(QTreeView* tree);

}  // namespace sup::gui::utils

#endif  // SUP_GUI_WIDGETS_STYLE_UTILS_H_
