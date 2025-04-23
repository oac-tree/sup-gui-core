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

#ifndef SUP_GUI_WIDGETS_SETTINGS_CALLBACKS_H_
#define SUP_GUI_WIDGETS_SETTINGS_CALLBACKS_H_

//! @file
//! Defines callbacks to talk with QSettings machinery. Introduced to decouple all classes whishing
//! to store their settings in persistent storage from QSettings and allow testability.

#include <QString>
#include <QVariant>
#include <functional>

namespace sup::gui
{

//! A function to write variant in a persistent storage using a given key.
using write_variant_func_t = std::function<void(const QString& key, const QVariant& value)>;

//! A function to read variant from a persistent storage using a given key.
using read_variant_func_t = std::function<QVariant(const QString& key)>;

/**
 * @brief Returns a function which shall be used to write setting in the main QSetting storage.
 *
 * Please note, that the function shall be used after the main window construction, when QSetting
 * machinery is fully initialized.
 */
write_variant_func_t GetSettingsWriteFunc();

/**
 * @brief Returns a function which shall be used to read setting from the main QSetting storage.
 *
 * Please note, that the function shall be used after the main window construction, when QSetting
 * machinery is fully initialized.
 */
read_variant_func_t GetSettingsReadFunc();

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_SETTINGS_CALLBACKS_H_
