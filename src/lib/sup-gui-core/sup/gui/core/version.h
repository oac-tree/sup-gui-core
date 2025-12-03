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

#ifndef SUP_GUI_CORE_VERSION_H_
#define SUP_GUI_CORE_VERSION_H_

//! Project version information as defined by CMake project

#include <cstdint>
#include <string>

namespace sup::gui
{

//! Returns major project version.
std::int32_t ProjectVersionMajor();

//! Returns minor project version.
std::int32_t ProjectVersionMinor();

//! Returns patch project version.
std::int32_t ProjectVersionPatch();

//! Returns project version string.
std::string ProjectVersion();

}  // namespace sup::gui

#endif  // SUP_GUI_CORE_VERSION_H_
