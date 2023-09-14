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

#ifndef SUP_GUI_MODEL_ANYVALUE_CONVERSION_UTILS_H_
#define SUP_GUI_MODEL_ANYVALUE_CONVERSION_UTILS_H_

#include <sup/gui/core/dto_types_fwd.h>

#include <mvvm/core/variant.h>

#include <memory>
#include <string>
#include <vector>

namespace sup::gui
{

class AnyValueItem;

const std::string kStructTypeName = "struct";
const std::string kArrayTypeName = "array";
const std::string kScalarTypeName = "scalar";

std::vector<std::string> GetMainTypeNames();

std::vector<std::string> GetScalarTypeNames();

//! Returns any_value type code from the name.
sup::dto::TypeCode GetTypeCode(const std::string& name);

//! Returns variant carrying type corresponding to AnyValue type name. The value of the variant will
//! be default initialized. See implementation for more explanations.
mvvm::variant_t GetVariantForAnyValueTypeName(const std::string& type_name);

//! Returns true if given name corresponds to a scalar.
bool IsScalarTypeName(const std::string& name);

//! Returns true if given name corresponds to a struct.
bool IsStructTypeName(const std::string& name);

//! Creates AnyValue from given item.
sup::dto::AnyValue CreateAnyValue(const AnyValueItem& item);

//! Creates AnyValueItem from given AnyValue.
std::unique_ptr<AnyValueItem> CreateItem(const sup::dto::AnyValue& any_value);

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_ANYVALUE_CONVERSION_UTILS_H_
