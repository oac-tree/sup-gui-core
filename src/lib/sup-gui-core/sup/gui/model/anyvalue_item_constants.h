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

#include <string>

namespace sup::gui::constants
{

const std::string kStructTypeName = "struct";
const std::string kArrayTypeName = "array";
const std::string kScalarTypeName = "scalar";
const std::string kEmptyTypeName = "empty";

const int kAnyTypeNameRole = 10;

const std::string kAnyValueChildrenTag = "kAnyValueChildrenTag";

const std::string kFieldNamePrefix = "field";
const std::string kElementNamePrefix = "element";
const std::string kAnyValueDefaultDisplayName = "value";

}  // namespace sup::gui::constants

#endif  // SUP_GUI_MODEL_ANYVALUE_ITEM_CONSTANTS_H_
