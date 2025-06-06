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

#ifndef SUP_GUI_MODEL_ANYVALUE_CONVERSION_UTILS_H_
#define SUP_GUI_MODEL_ANYVALUE_CONVERSION_UTILS_H_

//! @file
//! Helper functions to convert between AnyValue and AnyValueItem.

#include <sup/gui/core/dto_types_fwd.h>

#include <mvvm/core/variant.h>

#include <memory>
#include <string>
#include <vector>

namespace sup::gui
{

class AnyValueItem;

/**
 * @brief Returns list of available domain type names.
 */
std::vector<std::string> GetScalarTypeNames();

/**
 * @brief Returns domain's TypeCode from type name.
 */
sup::dto::TypeCode GetScalarTypeCode(const std::string& name);

/**
 * @brief Returns true if given name corresponds to a scalar.
 */
bool IsScalarTypeName(const std::string& name);

/**
 * @brief Returns true if given name corresponds to a struct.
 */
bool IsStructTypeName(const std::string& name);

/**
 * @brief Creates AnyValue from given item.
 */
sup::dto::AnyValue CreateAnyValue(const AnyValueItem& item);

/**
 * @brief Creates AnyValueItem from given AnyValue.
 */
std::unique_ptr<AnyValueItem> CreateAnyValueItem(const sup::dto::AnyValue& any_value);

/**
 * @brief Sets the data of AnyValueItem using scalar AnyValue.
 *
 * @param value The value to use.
 * @param item The item to update the data.
 *
 * @details Will throw if AnyValue is not a scalar. It is possible to change the type of the scalar.
 * AnyTypeName and underlying variant on board of AnyValueItem will be updated accordingly.
 */
void SetDataFromScalar(const anyvalue_t& value, AnyValueItem& item);

/**
 * @brief Returns scalar AnyValue from AnyValueItem containing a scalar.
 */
sup::dto::AnyValue GetAnyValueFromScalar(const AnyValueItem& item);

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_ANYVALUE_CONVERSION_UTILS_H_
