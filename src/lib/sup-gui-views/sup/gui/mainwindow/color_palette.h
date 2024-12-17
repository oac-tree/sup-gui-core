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

#ifndef SUP_GUI_MAINWINDOW_COLOR_PALETTE_H_
#define SUP_GUI_MAINWINDOW_COLOR_PALETTE_H_

//! @file
//! Collection of colors for light and dark themes.

#include <QColor>

namespace sup::gui
{

/**
 * @brief Returns the color of the main toolbar.
 */
QColor GetMainToolBarColor();

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_COLOR_PALETTE_H_
