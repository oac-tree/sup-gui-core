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

#include <mvvm/core/variant.h>

#include <QString>
#include <QVariant>

namespace mvvm
{
class ISessionModel;
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{

//! The name of the setting in QSettings file carrying XML representation of the root item of
//! SettingsModel.
const QString kRootSettingsModelName = "SettingsModel/xml";

class SettingsModel;

/**
 * @brief Assigns Qt variant to SessionItem.
 *
 * It is expected that variant was obtained from QSettings storage and contain a string
 * representation.
 */
void AssignStringBasedVariant(const QVariant& variant, mvvm::SessionItem& item);

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
 * @brief Writes application settings to persistent storage using the provided write function.
 *
 * Application settings are represented by SessionModel, where various scalar properties are
 * stored in compound items (see SettingsModel). The algorithm iterates through all property items
 * and saves their data roles. This method needs a write function.
 *
 * @param model The model that contains a collection of items representing settings.
 * @param A function to write into QSetting file.
 */
void WriteApplicationSettings(const mvvm::ISessionModel& model, write_variant_func_t func);

/**
 * @brief Save application settings to configured QSettings storage.
 *
 * The method should be used after InitCoreApplication call.
 *
 * Application settings are represented by SessionModel, where various scalar properties are
 * stored in compound items (see SettingsModel). The algorithm iterates through all items and saves
 * their data roles.
 *
 * @param model The model that contains a collection of items representing settings.
 */
void WriteApplicationSettings(const mvvm::ISessionModel& model);

/**
 * @brief Reads application settings from persistent storage using provided read function.
 *
 * Application settings are represented by SessionModel, where various scalar properties are
 * stored in compound items (see SettingsModel). The method iterates through all property items and
 * updates their data roles. The method doesn't add or remove property items.
 *
 * @param model The model that contains a collection of items representing settings.
 * @param A function to read from QSetting file.
 */
void ReadSettingsFromPersistentStorage(mvvm::ISessionModel& model, read_variant_func_t func);

/**
 * @brief Reads application settings from configured QSettings storage.
 *
 * The method should be used after InitCoreApplication call.
 *
 * Application settings are represented by SessionModel, where various scalar properties are
 * stored in compound items (see SettingsModel). The method iterates through all property items and
 * updates their data roles. The method doesn't add or remove property items.
 *
 * @param model The model that contains a collection of items representing settings.
 * @param A function to read from QSetting file.
 */
void ReadApplicationSettings(mvvm::ISessionModel& model);

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_SETTINGS_HELPER_H_
