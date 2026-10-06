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

#ifndef SUP_GUI_SETTINGS_IN_MEMORY_SETTINGS_STORAGE_H_
#define SUP_GUI_SETTINGS_IN_MEMORY_SETTINGS_STORAGE_H_

#include <sup/gui/settings/i_settings_storage.h>

#include <map>
#include <vector>

namespace sup::gui
{

/**
 * @brief The InMemorySettingsStorage class keeps settings in memory.
 */
class InMemorySettingsStorage : public ISettingsStorage
{
public:
  std::optional<mvvm::variant_t> Read(const std::string& key) const override;

  void Write(const std::string& key, const mvvm::variant_t& value) override;

  /**
   * @brief Returns all stored keys in alphabetical order.
   */
  std::vector<std::string> GetKeys() const;

private:
  std::map<std::string, mvvm::variant_t> m_values;
};

}  // namespace sup::gui

#endif  // SUP_GUI_SETTINGS_IN_MEMORY_SETTINGS_STORAGE_H_
