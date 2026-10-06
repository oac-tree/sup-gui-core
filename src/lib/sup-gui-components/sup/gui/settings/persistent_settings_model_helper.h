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

#ifndef SUP_GUI_SETTINGS_PERSISTENT_SETTINGS_MODEL_HELPER_H_
#define SUP_GUI_SETTINGS_PERSISTENT_SETTINGS_MODEL_HELPER_H_

#include <memory>

namespace sup::gui
{

class ISettingsManager;
class PersistentSettingsModel;

/**
 * @brief Creates the settings model populated from the application scope of the settings manager.
 *
 * @param manager The settings manager to read from.
 * @return The settings model.
 * @throw RuntimeException if no application storage is set.
 */
std::unique_ptr<PersistentSettingsModel> CreatePersistentSettingsModel(
    const ISettingsManager& manager);

/**
 * @brief Writes settings from the model to the application scope of the settings manager.
 *
 * Subscribers of written groups are notified.
 *
 * @param model The settings model to read from.
 * @param manager The settings manager to write to.
 * @throw RuntimeException if no application storage is set.
 */
void WritePersistentSettingsModel(const PersistentSettingsModel& model, ISettingsManager& manager);

}  // namespace sup::gui

#endif  // SUP_GUI_SETTINGS_PERSISTENT_SETTINGS_MODEL_HELPER_H_
