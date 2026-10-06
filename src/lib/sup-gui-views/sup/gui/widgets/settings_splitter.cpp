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

#include "settings_splitter.h"

#include <sup/gui/settings/i_settings_manager.h>

namespace sup::gui
{

SettingsSplitter::SettingsSplitter(std::string group, ISettingsManager& settings_manager,
                                   QWidget* parent_widget)
    : QSplitter(parent_widget), m_group(std::move(group)), m_settings_manager(settings_manager)
{
  m_provider_connection = m_settings_manager.RegisterProvider(
      m_group, SettingsScope::kApplication, [this]() { return GetSplitterInfo(); });
}

SplitterInfo SettingsSplitter::GetSplitterInfo() const
{
  SplitterInfo result;
  result.state = saveState();
  for (std::int32_t index = 0; index < count(); ++index)
  {
    result.visibility.push_back(static_cast<std::int32_t>(!widget(index)->isHidden()));
  }
  return result;
}

void SettingsSplitter::RestoreState()
{
  const auto info = m_settings_manager.Get<SplitterInfo>(m_group, SettingsScope::kApplication);
  if (!info.state.isEmpty())
  {
    (void)restoreState(info.state);
  }
  if (info.visibility.size() == static_cast<std::size_t>(count()))
  {
    for (std::int32_t index = 0; index < count(); ++index)
    {
      widget(index)->setVisible(static_cast<bool>(info.visibility.at(index)));
    }
  }
}

}  // namespace sup::gui
