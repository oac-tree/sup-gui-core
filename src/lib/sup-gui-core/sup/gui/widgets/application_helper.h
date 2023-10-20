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

#include <QFont>
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
 * @param verbose Provide output to the console regarding desktop geometry.
 */
void SetWindowStyle(const QString &app_style, int font_size, bool verbose = false);

/**
 * @brief Sets main GUI style of the application and main font size.
 * @param app_style The name of the GUI style from Qt's style factory.
 * @param font System font.
 * @param verbose Provide output to the console regarding desktop geometry.
 */
void SetWindowStyle(const QString &app_style, const QFont &font, bool verbose = false);

/**
 * @brief Sets up high DPI scaling.
 * @param scale_from_environment Use environment variables QT_SCALE_FACTOR,
 * QT_SCREEN_SCALE_FACTORS and QT_AUTO_SCREEN_SCALE_FACTOR to scale the UI.
 *
 * @details By default we use internal scaling mechanism and ignore values of scaling envionment
 * variables.
 */
void SetupHighDpiScaling(bool scale_from_environment = false);

/**
 * @brief Gets the current system user name.
 * @return The current system user name, or empty string if the user name cannot be retrieved.
 */
QString GetUserName();

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_APPLICATION_HELPER_H_
