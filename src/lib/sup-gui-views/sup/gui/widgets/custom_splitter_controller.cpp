/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "custom_splitter_controller.h"

#include <sup/gui/core/exceptions.h>

#include <QDebug>
#include <QSplitter>

namespace sup::gui
{

namespace
{

/**
 * @brief Returns flags representing children visibility.
 */
QList<int> GetChildrenVisibilityFlags(QSplitter *splitter)
{
  QList<int> result;
  for (int index = 0; index < splitter->count(); ++index)
  {
    result.append(static_cast<int>(splitter->widget(index)->isVisible()));
  }

  return result;
}

}  // namespace

CustomSplitterController::CustomSplitterController(const QString &setting_group_name,
                                                   QSplitter *splitter)
    : m_splitter(splitter), m_settings_group_name(setting_group_name)
{
  if (!m_splitter)
  {
    throw RuntimeException("Not initialised splitter");
  }

  if (m_settings_group_name.isEmpty())
  {
    throw RuntimeException("Setting group name can't be empty");
  }
}

void CustomSplitterController::ReadSettings(const read_variant_func_t &read_func) {}

void CustomSplitterController::WriteSettings(const write_variant_func_t &write_func)
{
  write_func(GetMainStateKey(), m_splitter->saveState());

  if (auto flags = GetChildrenVisibilityFlags(m_splitter); !flags.empty())
  {
    write_func(GetChildrenStateKey(), QVariant::fromValue(flags));
  }
}

QString CustomSplitterController::GetMainStateKey()
{
  return m_settings_group_name + "_state";
}

QString CustomSplitterController::GetChildrenStateKey()
{
  return m_settings_group_name + "_widgets";
}

}  // namespace sup::gui
