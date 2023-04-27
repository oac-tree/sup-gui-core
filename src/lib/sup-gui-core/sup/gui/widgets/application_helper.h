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

#ifndef SUP_GUI_WIDGETS_APPLICATION_HELPER_H_
#define SUP_GUI_WIDGETS_APPLICATION_HELPER_H_

//! Helper functions to start application.

#include <QString>

namespace sup::gui
{

/**
 * @brief Returns true if application is running in CODAC environment.
 */
bool IsOnCodac();

/**
 * @brief Initialize core COA application.
 * @param app_name The name of the executable.
 * @param version The string representing version in the form "major.minor.patch".
 */
void InitCoreApplication(const QString &app_name, const QString &version);

/**
 * @brief Sets main GUI style of the application and main font size.
 * @param app_style The name of the GUI style from Qt's style factory.
 * @param font_psize System font size in pt.
 */
void SetWindowStyle(const QString &app_style, int font_size);

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_APPLICATION_HELPER_H_
