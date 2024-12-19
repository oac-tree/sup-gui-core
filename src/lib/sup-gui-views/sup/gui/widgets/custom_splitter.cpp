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

#include "custom_splitter.h"

#include <QSettings>

namespace sup::gui
{

QString GetMainStateName(const QString& setting_name)
{
  return setting_name + "_state";
}

CustomSplitter::CustomSplitter(const QString& setting_name, QWidget* parent_widget)
    : QSplitter(parent_widget), m_setting_name(setting_name)
{
}

CustomSplitter::~CustomSplitter()
{
  WriteSettings();
}

void CustomSplitter::ReadSettings()
{
  if (m_setting_name.isEmpty())
  {
    return;
  }

  const QSettings settings;

  if (settings.contains(GetMainStateName(m_setting_name)))
  {
    restoreState(settings.value(GetMainStateName(m_setting_name)).toByteArray());
  }
}

void CustomSplitter::WriteSettings()
{
  if (m_setting_name.isEmpty())
  {
    return;
  }

  QSettings settings;
  settings.setValue(GetMainStateName(m_setting_name), saveState());
}

}  // namespace sup::gui
