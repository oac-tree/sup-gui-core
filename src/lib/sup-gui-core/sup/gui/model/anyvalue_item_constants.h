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

#ifndef SUP_GUI_MODEL_ANYVALUE_ITEM_CONSTANTS_H_
#define SUP_GUI_MODEL_ANYVALUE_ITEM_CONSTANTS_H_

namespace sup::gui::constants
{

constexpr auto kEmptyTypeName = "empty";
constexpr auto kBooleanTypeName = "bool";
constexpr auto kChar8TypeName = "char8";
constexpr auto kInt8TypeName = "int8";
constexpr auto kUInt8TypeName = "uint8";
constexpr auto kInt16TypeName = "int16";
constexpr auto kUInt16TypeName = "uint16";
constexpr auto kInt32TypeName = "int32";
constexpr auto kUInt32TypeName = "uint32";
constexpr auto kInt64TypeName = "int64";
constexpr auto kUInt64TypeName = "uint64";
constexpr auto kFloat32TypeName = "float32";
constexpr auto kFloat64TypeName = "float64";
constexpr auto kStringTypeName = "string";

constexpr auto kStructTypeName = "struct";
constexpr auto kArrayTypeName = "array";
constexpr auto kScalarTypeName = "scalar";

constexpr auto kAnyValueChildrenTag = "kAnyValueChildrenTag";
constexpr auto kAnyValueTypeTag = "kAnyValueTypeTag";

constexpr auto kFieldNamePrefix = "field";
constexpr auto kElementNamePrefix = "element";
constexpr auto kAnyValueDefaultDisplayName = "value";

}  // namespace sup::gui::constants

#endif  // SUP_GUI_MODEL_ANYVALUE_ITEM_CONSTANTS_H_
