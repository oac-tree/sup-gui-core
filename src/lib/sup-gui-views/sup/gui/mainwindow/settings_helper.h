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

#ifndef SUP_GUI_MAINWINDOW_SETTINGS_HELPER_H_
#define SUP_GUI_MAINWINDOW_SETTINGS_HELPER_H_

//!< @file
//!< Collection of helper function to access global application settings.

#include <sup/gui/widgets/settings_callbacks.h>

#include <QString>

namespace mvvm
{
class ISessionModel;
}

namespace sup::gui
{

//! The name of the setting in QSettings file carrying XML representation of the root item of
//! SettingsModel.
const QString kRootSettingsModelName = "SettingsModel/xml";

class SettingsModel;

/**
 * @brief Returns model with global application settings.
 */
const SettingsModel& GetGlobalSettings();

/**
 * @brief Saves given settings in persistent storage.
 */
void SaveSettingsInPersistentStorage(const SettingsModel& model);

/**
 * @brief Loads settings from persistent storage.
 *
 * If persistent storage doesn't contain any related records, the model will be left unchainged.
 */
void LoadSettingsFromPersistentStorage(SettingsModel& model);

/**
 * @brief Loads global settings from persistent storage.
 *
 * If persistent storage doesn't contain any related records, the model will be left unchainged.
 */
void ReadGlobalSettings();

/**
 * @brief Save settings to persistent storage using the provided write function.
 *
 * This is a simplified method that allows to write the model content in the QSettings file in
 * a human-readable format. Only the item's data role is saved. QSetting keys are constructed from
 * the display names of individual items.
 *
 * @param model The model that contains a collection of items representing settings.
 * @param Special function to write into QSetting file.
 */
void WriteSettingsToPersistentStorage(const mvvm::ISessionModel& model, write_variant_func_t func);

/**
 * @brief Read settings from persistent storage.
 *
 * This function reads the data from QSettings file. Only item's data role is updated. The model
 * should be already populated with proper settings items.
 *
 * @param model The model that contains a collection of items representing settings.
 * @param Special function to read from QSetting file.
 */
void ReadSettingsFromPersistentStorage(mvvm::ISessionModel& model, read_variant_func_t func);

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_SETTINGS_HELPER_H_
