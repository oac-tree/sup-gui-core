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

#include "version_helper.h"

#include "exceptions.h"

#include <mvvm/utils/string_utils.h>

std::tuple<int, int, int> sup::gui::ParseVersionString(const std::string& version_string)
{
  const std::string separator(".");
  auto parsed = mvvm::utils::SplitString(version_string, separator);

  if (parsed.size() != 3)
  {
    throw RuntimeException("Can't parse version string [" + version_string + "]");
  }

  std::vector<int> version_parts;
  for (const auto& str : parsed)
  {
    auto num = mvvm::utils::StringToInteger(str);
    if (!num.has_value())
    {
      throw RuntimeException("Can't parse version string [" + version_string + "]");
    }
    version_parts.push_back(num.value());
  }

  return std::make_tuple(version_parts[0], version_parts[1], version_parts[2]);
}
