/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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
 * @brief Size of mainw tolbar icons.
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
 */
QIcon GetIcon(const QString& icon_name);

//! Sets style for the tree to better look on CODAC systems.

/**
 * @brief Sest visual style of the tree to the "breeze"-like style, if necessary.
 *
 * On CODAC system's the default desktop style is "fusion". The tree has a flat look,  without
 * appealing lines connecting expand/collapse markers. Call to this method will try to fix this.
 */
void BeautifyTreeStyle(QTreeView* tree);

}  // namespace sup::gui::utils

#endif  // SUP_GUI_WIDGETS_STYLE_UTILS_H_
