/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_MODEL_ANYVALUE_UTILS_H_
#define SUP_GUI_MODEL_ANYVALUE_UTILS_H_

//! Collection of sup::dto::AnyValue related utility functions.

#include <sup/gui/core/dto_types_fwd.h>

#include <string>

namespace sup::gui
{

//! Returns JSON string representing AnyValue.
std::string AnyValueToJSONString(const anyvalue_t& value, bool is_pretty = false);

//! Returns JSON string representing AnyType.
std::string AnyTypeToJSONString(const anyvalue_t& value);

//! Returns JSON string representing the values of an AnyValue.
std::string ValuesToJSONString(const anyvalue_t& value);

sup::dto::AnyValue AnyValueFromJSONFile(const std::string& filename);

//! Returns AnyType from its JSON representation.
sup::dto::AnyType AnyTypeFromJSONString(const std::string& str);

//! Returns AnyValue from JSON representation of its type and value.
sup::dto::AnyValue AnyValueFromJSONString(const std::string& json_type,
                                          const std::string& json_value);

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_ANYVALUE_UTILS_H_
