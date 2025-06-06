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

#ifndef SUP_GUI_CORE_VERSION_HELPER_H_
#define SUP_GUI_CORE_VERSION_HELPER_H_

#include <string>
#include <tuple>

namespace sup::gui
{

/**
 * @brief Parses version string "major.minor.patch" to three integers.
 *
 * Will throw if format ot string differs.
 */
std::tuple<int, int, int> ParseVersionString(const std::string& version_string);

/**
 * @brief Checks if two version strings has same major and minor versions in a version string.
 */
bool HasSameMajorMinorVersion(const std::string& str1, const std::string& str2);

}  // namespace sup::gui

#endif  // SUP_GUI_CORE_VERSION_HELPER_H_
