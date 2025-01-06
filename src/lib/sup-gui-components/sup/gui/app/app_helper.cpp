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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "app_helper.h"

#include <mvvm/utils/string_utils.h>

#include <QSize>
#include <QString>

namespace sup::gui
{

bool IsOnCodac()
{
#ifdef CODAC_FOUND
  return true;
#else
  return false;
#endif
}

std::optional<QSize> ParseSizeString(const QString &text)
{
  auto parts = mvvm::utils::SplitString(text.toStdString(), "x");

  if (parts.size() != 2)
  {
    return {};
  }

  auto x = mvvm::utils::StringToInteger(parts.at(0));
  auto y = mvvm::utils::StringToInteger(parts.at(1));

  return x.has_value() && y.has_value() ? QSize(x.value(), y.value()) : std::optional<QSize>{};
}

}  // namespace sup::gui
