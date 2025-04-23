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

#ifndef SUP_GUI_CORE_DTO_TYPES_FWD_H_
#define SUP_GUI_CORE_DTO_TYPES_FWD_H_

//! Forward declaration of AnyValue related types.

#include <sup/dto/basic_scalar_types.h>

namespace sup::dto
{
class AnyValue;
class AnyType;
class AnyTypeRegistry;
enum class TypeCode : sup::dto::uint32;
}  // namespace sup::dto

using anyvalue_t = sup::dto::AnyValue;
using anytype_t = sup::dto::AnyType;
using anytype_registry_t = sup::dto::AnyTypeRegistry;

#endif  // SUP_GUI_CORE_DTO_TYPES_FWD_H_
