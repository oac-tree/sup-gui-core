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

#include "anyvalue_conversion_utils.h"

#include "anyvalue_item.h"
#include "anyvalue_item_builder.h"
#include "anyvalue_item_constants.h"
#include "domain_anyvalue_builder.h"
#include "scalar_conversion_utils.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>

#include <algorithm>
#include <map>

namespace
{

/**
 * @brief Constructs vector of scalar type names.
 */
std::vector<std::string> CreateScalarTypeNames()
{
  std::vector<std::string> result;

  static const auto scalar_definitions = sup::dto::ScalarTypeDefinitions();
  auto on_element = [](const auto& pair) { return pair.second; };
  (void)std::transform(scalar_definitions.begin(), scalar_definitions.end(),
                       std::back_inserter(result), on_element);
  return result;
}

/**
 * @brief Constructs a map to convert scalar type code to name.
 */
std::map<sup::dto::TypeCode, std::string> CreateScalarTypeCodeNameMap()
{
  std::map<sup::dto::TypeCode, std::string> result;

  static const auto scalar_definitions = sup::dto::ScalarTypeDefinitions();
  auto on_element = [&result](const auto& pair) { return result.insert(pair); };
  (void)std::for_each(scalar_definitions.begin(), scalar_definitions.end(), on_element);
  return result;
}
}  // namespace

namespace sup::gui
{

std::vector<std::string> GetScalarTypeNames()
{
  static const auto names = CreateScalarTypeNames();
  return names;
}

sup::dto::TypeCode GetScalarTypeCode(const std::string& name)
{
  static const auto typecode_map = CreateScalarTypeCodeNameMap();

  auto iter = std::find_if(typecode_map.begin(), typecode_map.end(),
                           [name](const auto& item) { return item.second == name; });
  if (iter == typecode_map.end())
  {
    throw RuntimeException("Error! Can't find TypeCode corresponding to scalar name [" + name
                           + "]");
  }
  return iter->first;
}

bool IsScalarTypeName(const std::string& name)
{
  static const std::vector<std::string> expected_names = GetScalarTypeNames();
  return std::find(expected_names.begin(), expected_names.end(), name) != expected_names.end();
}

bool IsStructTypeName(const std::string& name)
{
  return name == constants::kStructTypeName;
}

sup::dto::AnyValue CreateAnyValue(const AnyValueItem& item)
{
  DomainAnyValueBuilder builder(item);
  return builder.GetAnyValue();
}

std::unique_ptr<AnyValueItem> CreateAnyValueItem(const sup::dto::AnyValue& any_value)
{
  AnyValueItemBuilder builder;
  sup::dto::SerializeAnyValue(any_value, builder);
  return builder.MoveAnyValueItem();
}

void SetDataFromScalar(const anyvalue_t& value, AnyValueItem& item)
{
  auto variant = GetVariantFromScalar(value);

  if (item.GetAnyTypeName() != value.GetTypeName())
  {
    (void)item.SetData(
        mvvm::variant_t());  // it resets data on board and allow to change variant type
    item.SetAnyTypeName(value.GetTypeName());
  }

  (void)item.SetData(variant);
}

sup::dto::AnyValue GetAnyValueFromScalar(const AnyValueItem& item)
{
  return GetAnyValueFromScalar(item.Data());
}

}  // namespace sup::gui
