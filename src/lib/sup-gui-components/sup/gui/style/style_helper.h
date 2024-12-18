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

#include <QWidget>

class QIcon;
class QTreeView;
class QIconEngine;

namespace sup::gui::utils
{

/**
 * @brief The AppIconColorFlavor enum defines the dark/light type of the icon.
 */
enum AppIconColorFlavor
{
  kUnspecified,    //!< dark or light will be automatically deduced from current theme
  kForDarkThemes,  //!< icon is intended for dark background
  kForLightThemes  //!< icon is intended for light background
};

/**
 * @brief Registers given resource file name, and path in it, under the given alias name.
 *
 * @param file_name The name of the resource file.
 * @param path Path in this resource file.
 * @param alias Alias that can be used to find resources.
 *
 * @details Example:
 * For resource file myresource.qrc with the following content:
 * <qresource prefix="/core">
 *    <file>icon_folder/animation.svg</file>
 * </qresource>
 *
 * the call to RegisterResource("myresource.qrc", ":/core/icon_folder", "icons") will allow to
 * access icons as
 *
 * QIcon icon("icons:animation.svg");
 * instead of
 * QIcon icon(":/core/icon_folder/animation.svg");
 */
void RegisterResource(const QString& file_name, const QString& path, const QString& alias);

/**
 * @brief Register icons of sup-gui-core package for global usage.
 *
 * Icons can be used via path alias, @see RegisterResource() comments.
 */
void RegisterCoreIconAlias(const QString& alias = "icons");

/**
 * @brief Checks if main application theme is dark.
 */
bool IsDarkTheme();

/**
 * @brief Returns resource name for given icon name.
 *
 * Example:
 * for "menu" -> ":/icons/menu.svg"
 * for "menu.svg" -> ":/icons/menu.svg"
 * for "menu.png" -> ":/icons/menu.png"
 */
QString GetResourceName(const QString& icon_name);

/**
 * @brief Returns icon color corresponding to a given icon flavor.
 */
QColor GetIconBaseColor(AppIconColorFlavor icon_flavor);

/**
 * @brief Returns an icon for a given icon name.
 *
 * If the name doesn't contain an extension, will assume an existence of svg file in the resource
 * folder.
 *
 * @param icon_type The desired dark/light type of the icon.
 *
 * @note Svg icons are special. We use flat-style monochrome icons. When icon_type is
 * unspecified, icons will be rendered according to the current theme's dark/light flavor. Icons
 * for the dark theme will be rendered as almost white, icons for the light theme will be rendered
 * as almost black. User can still specify the desired icon_flavor thus overriding the current
 * theme's dark/light style.
 */
QIcon GetIcon(const QString& icon_name,
              AppIconColorFlavor icon_flavor = AppIconColorFlavor::kUnspecified);

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
