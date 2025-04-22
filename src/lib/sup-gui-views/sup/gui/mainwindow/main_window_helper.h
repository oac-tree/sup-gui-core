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

#ifndef SUP_GUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_
#define SUP_GUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_

//! @file
//! Helper functions to start application.

#include <QFont>
#include <QString>
#include <cstdint>
#include <optional>

namespace sup::gui
{

/**
 * @brief Initialize core COA application.
 *
 * @param app_name The name of the executable.
 * @param version The string representing version in the form "major.minor.patch".
 */
void InitCoreApplication(const QString &app_name, const QString &version);

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
 * @brief Sets application from options.
 *
 * @param font_size_hint System font size in pt.
 * @param app_style The name of the GUI style from Qt's style factory.
 * @param verbose Provide output to the console regarding desktop geometry.
 */
void SetupApplication(std::int32_t font_size_hint, const QString &app_style, bool verbose = false);

/**
 * @brief Cleanup globally registered menus, actions, and exit application.
 *
 * This method should be called on application exit, when it is expected that it will be restarted
 * after in the same event loop.
 */
void ShutdownApplication();

/**
 * @brief Sets main GUI style of the application and main font size.
 *
 * @param app_style The name of the GUI style from Qt's style factory.
 */
void SetWindowStyle(const QString &app_style);

/**
 * @brief Sets application font size.
 *
 * @param font_size_hint System font size in pt.
 *
 * @details If font size is negative, will look in application settings. If no font is not found,
 * will proceed without font change.
 */
void SetApplicationFont(std::int32_t font_size_hint);

/**
 * @brief Gets the current system user name.
 * @return The current system user name, or empty string if the user name cannot be retrieved.
 */
QString GetUserName();

/**
 * @brief Return application font stored in settings.
 *
 * The method should be called after InitCoreApplication.
 */
std::optional<QFont> GetAppFontFromSettings();

/**
 * @brief Saves application font in settings
 *
 * The method should be called after InitCoreApplication.
 */
void SaveAppFontInSettings(const QFont &font);

/**
 * @brief Checks if the application has a settings file and this settings can be used for the
 * given application version.
 *
 * There should be a setting file on disk, it should contain the version string, and this version
 * string should match app_version string in "minor.major" parts. This method should be always
 * called after InitCoreApplication.
 */
bool HasValidAppSettings(const std::string &app_version);

/**
 * @brief Opens message box with the question if we should reset application settings and restart
 *
 * @return Truth if reset and restart was requested, false otherwise.
 */
bool ShouldResetSettingsAndRestart();

/**
 * @brief Summons dialog to change system font.
 */
bool SummonChangeSystemFontDialog();

/**
 * @brief Checks if we are without monitor (i.e. Jenkins on CODAC)
 */
bool IsHeadlessMode();

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_
