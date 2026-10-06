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

#ifndef SUP_GUI_SETTINGS_PROJECT_SETTINGS_STORAGE_H_
#define SUP_GUI_SETTINGS_PROJECT_SETTINGS_STORAGE_H_

#include <sup/gui/settings/i_settings_storage.h>

#include <functional>

namespace mvvm
{
class ISessionModel;
}  // namespace mvvm

namespace sup::gui
{

/**
 * @brief The ProjectSettingsStorage class keeps settings in the project model.
 *
 * Values are stored in SettingsContainerItem at the top level of the model. The item is created
 * on the first write.
 */
class ProjectSettingsStorage : public ISettingsStorage
{
public:
  //! A function returning the current project model, or nullptr if there is no project.
  using model_func_t = std::function<mvvm::ISessionModel*()>;

  explicit ProjectSettingsStorage(model_func_t model_func);

  /**
   * @brief Returns the value of the key, or nullopt if the key or the model is absent.
   */
  std::optional<mvvm::variant_t> Read(const std::string& key) const override;

  /**
   * @brief Writes the value of the key. Does nothing if the model is absent.
   */
  void Write(const std::string& key, const mvvm::variant_t& value) override;

private:
  model_func_t m_model_func;
};

}  // namespace sup::gui

#endif  // SUP_GUI_SETTINGS_PROJECT_SETTINGS_STORAGE_H_
