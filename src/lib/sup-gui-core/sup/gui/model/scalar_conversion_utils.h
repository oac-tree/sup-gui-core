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

#ifndef SUP_GUI_MODEL_SCALAR_CONVERSION_UTILS_H_
#define SUP_GUI_MODEL_SCALAR_CONVERSION_UTILS_H_

//! @file
//! Utility functions to convert scalar AnyValue to variant_t and back.

#include <sup/gui/core/dto_types_fwd.h>

#include <mvvm/core/variant.h>

namespace sup::gui
{

class AnyValueItem;

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
 * @brief Return scalar-like variant from AnyValue rpresenting a scalar.
 */
mvvm::variant_t GetVariantFromScalar(const anyvalue_t& value);

/**
 * @brief Returns scalar AnyValue from AnyValueItem containing a scalar.
 */
sup::dto::AnyValue GetAnyValueFromScalar(const AnyValueItem& item);

/**
 * @brief Returns scalar AnyValue from variant_t containing a scalar.
 */
sup::dto::AnyValue GetAnyValueFromScalar(const mvvm::variant_t& variant);

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_SCALAR_CONVERSION_UTILS_H_
