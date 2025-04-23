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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "version_helper.h"

#include "sup_gui_core_exceptions.h"

#include <mvvm/utils/string_utils.h>

namespace sup::gui
{

std::tuple<int, int, int> ParseVersionString(const std::string& version_string)
{
  const std::string separator(".");

  std::vector<int> version_parts;
  for (const auto& str : mvvm::utils::SplitString(version_string, separator))
  {
    if (auto num = mvvm::utils::StringToInteger(str); num.has_value())
    {
      version_parts.push_back(num.value());
    }
  }

  if (version_parts.size() != 3)
  {
    throw RuntimeException("Can't parse version string [" + version_string + "]");
  }

  return std::make_tuple(version_parts[0], version_parts[1], version_parts[2]);
}

bool HasSameMajorMinorVersion(const std::string& str1, const std::string& str2)
{
  auto parts1 = ParseVersionString(str1);
  auto parts2 = ParseVersionString(str2);

  return std::get<0>(parts1) == std::get<0>(parts2) && std::get<1>(parts1) == std::get<1>(parts2);
}

}  // namespace sup::gui
