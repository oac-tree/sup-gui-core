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

#ifndef SUP_GUI_APP_MAIN_WINDOW_TYPES_H_
#define SUP_GUI_APP_MAIN_WINDOW_TYPES_H_

namespace sup::gui
{

/**
 * @brief The AppExitCode enum defines exit codes of the application.
 */
enum AppExitCode
{
  NormalExit,
  Restart,
  CleanSettingsAndRestart
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_MAIN_WINDOW_TYPES_H_
