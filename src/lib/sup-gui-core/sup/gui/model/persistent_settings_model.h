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

#ifndef SUP_GUI_MODEL_PERSISTENT_SETTINGS_MODEL_H_
#define SUP_GUI_MODEL_PERSISTENT_SETTINGS_MODEL_H_

#include <sup/gui/model/settings_constants.h>
#include <sup/gui/model/common_settings_item.h>

#include <mvvm/model/application_model.h>

namespace sup::gui
{

/**
 * @brief Contains editable application settings common across sessions.
 *
 * These settings can be: flags denoting whether undo/redo is enabled, lists of plugins to load,
 * etc.
 *
 * The model contains various editable setting items, which are written in persistent storage thanks
 * to QSettings machinery.
 */
class PersistentSettingsModel : public mvvm::ApplicationModel
{
public:
  PersistentSettingsModel();
  PersistentSettingsModel(std::string model_type, std::shared_ptr<mvvm::ItemPool> pool);

  std::unique_ptr<ISessionModel> Clone() const override;

  /**
   * @details Clears the model and populates it with the default settings.
   */
  void Clear() override;

  std::vector<const mvvm::CompoundItem*> GetSettingsItems() const;

  std::vector<mvvm::CompoundItem*> GetSettingsItems();

  /**
   * @brief Returns data inside property item registered under the given setting name.
   */
  template <typename T = mvvm::variant_t>
  T Data(const std::string& setting_name) const;

private:
  void PopulateModel();
};

template <typename T>
inline T PersistentSettingsModel::Data(const std::string& setting_name) const
{
  // for the moment we assume that there is only one settings item, might change in the future
  return GetSettingsItems().at(0)->Property<T>(setting_name);
}

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_PERSISTENT_SETTINGS_MODEL_H_
