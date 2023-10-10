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

#include "anyvalue_conversion_utils.h"

#include "anyvalue_item.h"
#include "anyvalue_item_builder.h"
#include "domain_anyvalue_builder.h"
#include "scalar_conversion_utils.h"

#include <sup/dto/anytype.h>
#include <sup/dto/anytype_registry.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/basic_scalar_types.h>

#include <algorithm>
#include <fstream>
#include <functional>
#include <map>
#include <sstream>
#include <stdexcept>

namespace
{

std::vector<std::string> CreateScalarTypeNames()
{
  const auto scalar_definitions = sup::dto::ScalarTypeDefinitions();
  std::vector<std::string> result;
  auto on_element = [](const auto& pair) { return pair.second; };
  std::transform(scalar_definitions.begin(), scalar_definitions.end(), std::back_inserter(result),
                 on_element);
  return result;
}

const std::map<sup::dto::TypeCode, std::string> kTypeCodeNameMap = {
    {sup::dto::TypeCode::Empty, sup::dto::kEmptyTypeName},
    {sup::dto::TypeCode::Struct, sup::gui::kStructTypeName},
    {sup::dto::TypeCode::Array, sup::gui::kArrayTypeName},
    {sup::dto::TypeCode::Bool, sup::dto::kBooleanTypeName},
    {sup::dto::TypeCode::Char8, sup::dto::kChar8TypeName},
    {sup::dto::TypeCode::Int8, sup::dto::kInt8TypeName},
    {sup::dto::TypeCode::UInt8, sup::dto::kUInt8TypeName},
    {sup::dto::TypeCode::Int16, sup::dto::kInt16TypeName},
    {sup::dto::TypeCode::UInt16, sup::dto::kUInt16TypeName},
    {sup::dto::TypeCode::Int32, sup::dto::kInt32TypeName},
    {sup::dto::TypeCode::UInt32, sup::dto::kUInt32TypeName},
    {sup::dto::TypeCode::Int64, sup::dto::kInt64TypeName},
    {sup::dto::TypeCode::UInt64, sup::dto::kUInt64TypeName},
    {sup::dto::TypeCode::Float32, sup::dto::kFloat32TypeName},
    {sup::dto::TypeCode::Float64, sup::dto::kFloat64TypeName},
    {sup::dto::TypeCode::String, sup::dto::kStringTypeName}};

std::string GetName(sup::dto::TypeCode code)
{
  auto iter = kTypeCodeNameMap.find(code);
  return iter == kTypeCodeNameMap.end() ? std::string() : iter->second;
}

}  // namespace

namespace sup::gui
{

std::vector<std::string> GetMainTypeNames()
{
  return {kStructTypeName, kArrayTypeName, kScalarTypeName};
}

std::vector<std::string> GetScalarTypeNames()
{
  static const auto names = CreateScalarTypeNames();
  return names;
}

sup::dto::TypeCode GetTypeCode(const std::string& name)
{
  auto iter = std::find_if(kTypeCodeNameMap.begin(), kTypeCodeNameMap.end(),
                           [name](auto item) { return item.second == name; });
  if (iter == kTypeCodeNameMap.end())
  {
    throw std::runtime_error("Error in TypeCode");
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
  return name == kStructTypeName;
}

sup::dto::AnyValue CreateAnyValue(const AnyValueItem& item)
{
  DomainAnyValueBuilder builder(item);
  return builder.GetAnyValue();
}

std::unique_ptr<AnyValueItem> CreateItem(const sup::dto::AnyValue& any_value)
{
  AnyValueItemBuilder builder;
  sup::dto::SerializeAnyValue(any_value, builder);
  return std::move(builder.MoveAnyValueItem());
}

void SetDataFromScalar(const anyvalue_t& value, AnyValueItem& item)
{
  auto variant = GetVariantFromScalar(value);

  if (item.GetAnyTypeName() != value.GetTypeName())
  {
    item.SetData(mvvm::variant_t());  // it resets data on board and allow to change variant type
    item.SetAnyTypeName(value.GetTypeName());
  }

  item.SetData(variant);
}

sup::dto::AnyValue GetAnyValueFromScalar(const AnyValueItem& item)
{
  return GetAnyValueFromScalar(item.Data());
}

}  // namespace sup::gui
