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

#ifndef SUP_GUI_MODEL_ANYVALUE_UTILS_H_
#define SUP_GUI_MODEL_ANYVALUE_UTILS_H_

//! @file
//! Collection of sup::dto::AnyValue related utility functions.

#include <sup/gui/core/dto_types_fwd.h>

#include <string>

namespace sup::gui
{

/**
 * @brief Returns JSON string representing AnyValue.
 */
std::string AnyValueToJSONString(const anyvalue_t& value, bool is_pretty = false);

/**
 * @brief Returns JSON string representing AnyType.
 */
std::string AnyTypeToJSONString(const anyvalue_t& value);

/**
 * @brief Returns JSON string representing the values of an AnyValue.
 */
std::string ValuesToJSONString(const anyvalue_t& value);

/**
 * @brief Returns AnyValue as serialized in a JSON file.
 */
sup::dto::AnyValue AnyValueFromJSONFile(const std::string& filename,
                                        const anytype_registry_t* registry = nullptr);

/**
 * @brief Returns AnyType from its JSON representation.
 */
sup::dto::AnyType AnyTypeFromJSONString(const std::string& str,
                                        const anytype_registry_t* registry = nullptr);

/**
 * @brief Returns AnyValue from its JSON representation.
 */
sup::dto::AnyValue AnyValueFromJSONString(const std::string& str,
                                          const anytype_registry_t* registry = nullptr);

/**
 * @brief Returns AnyValue from AnyType and JSON representation of its value.
 */
sup::dto::AnyValue AnyValueFromJSONString(const sup::dto::AnyType& anytype,
                                          const std::string& value_str);

/**
 * @brief Returns AnyValue from JSON representation of its type and value.
 */
sup::dto::AnyValue AnyValueFromJSONString(const std::string& type_str,
                                          const std::string& value_str);

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_ANYVALUE_UTILS_H_
