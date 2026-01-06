/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef SUP_GUI_MAINWINDOW_STATUS_BAR_HELPER_H_
#define SUP_GUI_MAINWINDOW_STATUS_BAR_HELPER_H_

//! @file
//! Collection of helper functions for main QStatusBar.

#include <QString>

class QToolButton;
class QStatusBar;
class QAction;

namespace sup::gui
{

/**
 * @brief Add stretch widget to permanent section of the given QStatusBar.
 */
void AddPermanentStretch(QStatusBar* status_bar, int stretch = 1);

/**
 * @brief Setup status bar button.
 *
 * It will tune tool button appearance intended for the main application status bar, and it will
 * connects the button with proxy action.
 *
 * @param button The button to setup.
 * @param action The action which will be associated with this tool button.
 */
void SetupStatusBarButton(QToolButton* button, QAction* action);

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_STATUS_BAR_HELPER_H_
