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

#include "project_settings_storage.h"

#include <sup/gui/model/settings_container_item.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/model_utils.h>

namespace sup::gui
{

ProjectSettingsStorage::ProjectSettingsStorage(model_func_t model_func)
    : m_model_func(std::move(model_func))
{
}

std::optional<mvvm::variant_t> ProjectSettingsStorage::Read(const std::string& key) const
{
  auto model = m_model_func();
  if (model == nullptr)
  {
    return {};
  }

  auto item = mvvm::utils::GetTopItem<SettingsContainerItem>(*model);
  return item == nullptr ? std::nullopt : item->GetValue(key);
}

void ProjectSettingsStorage::Write(const std::string& key, const mvvm::variant_t& value)
{
  auto model = m_model_func();
  if (model == nullptr)
  {
    return;
  }

  auto item = mvvm::utils::GetTopItem<SettingsContainerItem>(*model);
  if (item == nullptr)
  {
    item = model->InsertItem<SettingsContainerItem>();
  }
  item->SetValue(key, value);
}

}  // namespace sup::gui
