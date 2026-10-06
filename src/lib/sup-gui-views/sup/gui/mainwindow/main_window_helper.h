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

#ifndef SUP_GUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_
#define SUP_GUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_

//! @file
//! Helper functions to start application.

#include <QFont>
#include <QString>
#include <cstdint>
#include <optional>
#include <string>

namespace sup::gui
{

class ISettingsManager;

/**
 * @brief Initialize core COA application.
 *
 * @param app_name The name of the executable.
 * @param version The string representing version in the form "major.minor.patch".
 */
void InitCoreApplication(const QString& app_name, const QString& version);

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
void SetupApplication(std::int32_t font_size_hint, const QString& app_style, bool verbose = false);

/**
 * @brief Sets main GUI style of the application and main font size.
 *
 * @param app_style The name of the GUI style from Qt's style factory.
 */
void SetWindowStyle(const QString& app_style);

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
void SaveAppFontInSettings(const QFont& font);

/**
 * @brief Checks if the application has a settings file and this settings can be used for the
 * given application version.
 *
 * There should be a setting file on disk, it should contain the version string, and this version
 * string should match app_version string in "minor.major" parts. This method should be always
 * called after InitCoreApplication.
 */
bool HasValidAppSettings(const std::string& app_version);

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
 * @brief Opens dialog and let user to select the file to open.
 *
 * Will invoke file selection dialog and return user choice. The name will be empty if the user has
 * canceled the dialog. The last working directory will be saved in the persistence settings under
 * the given key (if non-empty), so the next time dialog will be opened there.
 *
 * @param filter Filter string (i.e. "All JSON files (*.json *.JSON)")
 * @param parent The parent widget will be used to center file selection dialog.
 * @param key The key to store the setting in the global application persistent setting file.
 * @return Selected file name. Will be empty if dialog was canceled.
 */
QString GetDialogFileNameToOpen(const QString& filter, QWidget* parent = nullptr,
                                const QString& key = {});

/**
 * @brief Opens dialog and let user to select the file to save.
 *
 * Will invoke file selection dialog and return user choice. The name will be empty if the user has
 * canceled the dialog. The last working directory will be saved in the persistence settings under
 * the given key (if non-empty), so the next time dialog will be opened there.
 *
 * @param name_prototype For example "untitled.json"
 * @param filter Filter string (i.e. "All JSON files (*.json *.JSON)")
 * @param parent The parent widget will be used to center file selection dialog.
 * @param key The key to store the setting in the global application persistent setting file.
 * @return Selected file name. Will be empty if dialog was canceled.
 */
QString GetDialogFileNameToSave(const QString& name_prototype, const QString& filter,
                                QWidget* parent = nullptr, const QString& key = {});

/**
 * @brief Opens dialog and lets the user select the file to open.
 *
 * The dialog starts in the working directory stored in application settings. The directory of the
 * selected file is stored back.
 *
 * @param filter Filter string (i.e. "All JSON files (*.json *.JSON)")
 * @param parent The parent widget will be used to center file selection dialog.
 * @param settings_manager The manager to store the working directory.
 * @param group The settings group of the working directory.
 * @return Selected file name. Will be empty if dialog was canceled.
 */
QString GetDialogFileNameToOpen(const QString& filter, QWidget* parent,
                                ISettingsManager& settings_manager, const std::string& group);

/**
 * @brief Opens dialog and lets the user select the file to save.
 *
 * The dialog starts in the working directory stored in application settings. The directory of the
 * selected file is stored back.
 *
 * @param name_prototype For example "untitled.json"
 * @param filter Filter string (i.e. "All JSON files (*.json *.JSON)")
 * @param parent The parent widget will be used to center file selection dialog.
 * @param settings_manager The manager to store the working directory.
 * @param group The settings group of the working directory.
 * @return Selected file name. Will be empty if dialog was canceled.
 */
QString GetDialogFileNameToSave(const QString& name_prototype, const QString& filter,
                                QWidget* parent, ISettingsManager& settings_manager,
                                const std::string& group);

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_MAIN_WINDOW_HELPER_H_
