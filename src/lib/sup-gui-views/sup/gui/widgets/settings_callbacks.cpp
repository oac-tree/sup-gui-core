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

#include "settings_callbacks.h"

#include <QSettings>

namespace sup::gui
{

write_variant_func_t GetSettingsWriteFunc()
{
  return [](const QString& key, const QVariant& value)
  {
    QSettings settings;
    settings.setValue(key, value);
  };
}

read_variant_func_t GetSettingsReadFunc()
{
  return [](const QString& key) -> QVariant
  {
    const QSettings settings;
    return settings.value(key);
  };
}

}  // namespace sup::gui
