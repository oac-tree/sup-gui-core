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

#ifndef SUP_GUI_MODEL_SCALAR_CONVERSION_UTILS_H_
#define SUP_GUI_MODEL_SCALAR_CONVERSION_UTILS_H_

//! @file
//! Utility functions to convert scalar AnyValue to variant_t and back.

#include <sup/gui/core/dto_types_fwd.h>

#include <mvvm/core/variant.h>

namespace sup::gui
{

/**
 * @brief Return scalar-like variant from AnyValue rpresenting a scalar.
 */
mvvm::variant_t GetVariantFromScalar(const anyvalue_t& value);

/**
 * @brief Returns scalar AnyValue from variant_t containing a scalar.
 */
sup::dto::AnyValue GetAnyValueFromScalar(const mvvm::variant_t& variant);

/**
 * @brief Returns scalar-like variant corresponding to a given AnyValue type name. The value
 * of the variant will be default initialized.
 */
mvvm::variant_t GetVariantFromScalarTypeName(const std::string& type_name);

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_SCALAR_CONVERSION_UTILS_H_
