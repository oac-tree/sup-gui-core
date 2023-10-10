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

#include "scalar_conversion_utils.h"

#include "anyvalue_conversion_utils.h"
#include "anyvalue_item.h"

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/basic_scalar_types.h>

#include <functional>
#include <map>
#include <stdexcept>

namespace sup::gui
{

/**
 * @brief Assigns scalar value from variant_t to pre-created AnyValue value.
 */
template <typename T>
void AssignToAnyValueScalar(const mvvm::variant_t &variant, sup::dto::AnyValue &any_value)
{
  any_value.ConvertFrom(std::get<T>(variant));
}

template <typename T>
mvvm::variant_t SetDataFromScalarT(const sup::dto::AnyValue &anyvalue)
{
  T val = anyvalue.As<T>();
  return {val};
}

void SetDataFromScalar(const anyvalue_t &value, AnyValueItem &item)
{
  using sup::dto::TypeCode;
  using function_t = std::function<mvvm::variant_t(const sup::dto::AnyValue &anyvalue)>;
  static std::map<TypeCode, function_t> conversion_map{
      {TypeCode::Bool, SetDataFromScalarT<sup::dto::boolean>},
      {TypeCode::Char8, SetDataFromScalarT<sup::dto::char8>},
      {TypeCode::Int8, SetDataFromScalarT<sup::dto::int8>},
      {TypeCode::UInt8, SetDataFromScalarT<sup::dto::uint8>},
      {TypeCode::Int16, SetDataFromScalarT<sup::dto::int16>},
      {TypeCode::UInt16, SetDataFromScalarT<sup::dto::uint16>},
      {TypeCode::Int32, SetDataFromScalarT<sup::dto::int32>},
      {TypeCode::UInt32, SetDataFromScalarT<sup::dto::uint32>},
      {TypeCode::Int64, SetDataFromScalarT<sup::dto::int64>},
      {TypeCode::UInt64, SetDataFromScalarT<sup::dto::uint64>},
      {TypeCode::Float32, SetDataFromScalarT<sup::dto::float32>},
      {TypeCode::Float64, SetDataFromScalarT<sup::dto::float64>},
      {TypeCode::String, SetDataFromScalarT<std::string>}};

  auto iter = conversion_map.find(value.GetTypeCode());
  if (item.GetAnyTypeName() != value.GetTypeName())
  {
    item.SetData(mvvm::variant_t());  // it resets data on board and allow to change variant type
    item.SetAnyTypeName(value.GetTypeName());
  }
  if (iter == conversion_map.end())
  {
    throw std::runtime_error("Not a known scalar type code");
  }
  auto variant = iter->second(value);
  item.SetData(variant);
}

sup::dto::AnyValue GetAnyValueFromScalar(const AnyValueItem &item)
{
  using anyvalue_function_t =
      std::function<void(const mvvm::variant_t &variant, sup::dto::AnyValue &anyvalue)>;

  //! Correspondance of AnyValue type code to PVXS value function to assign scalars.
  const static std::map<sup::dto::TypeCode, anyvalue_function_t> kAssignToAnyValueScalarMap = {
      {sup::dto::TypeCode::Bool, AssignToAnyValueScalar<mvvm::boolean>},
      {sup::dto::TypeCode::Char8, AssignToAnyValueScalar<mvvm::char8>},
      {sup::dto::TypeCode::Int8, AssignToAnyValueScalar<mvvm::int8>},
      {sup::dto::TypeCode::UInt8, AssignToAnyValueScalar<mvvm::uint8>},
      {sup::dto::TypeCode::Int16, AssignToAnyValueScalar<mvvm::int16>},
      {sup::dto::TypeCode::UInt16, AssignToAnyValueScalar<mvvm::uint16>},
      {sup::dto::TypeCode::Int32, AssignToAnyValueScalar<mvvm::int32>},
      {sup::dto::TypeCode::UInt32, AssignToAnyValueScalar<mvvm::uint32>},
      {sup::dto::TypeCode::Int64, AssignToAnyValueScalar<mvvm::int64>},
      {sup::dto::TypeCode::UInt64, AssignToAnyValueScalar<mvvm::uint64>},
      {sup::dto::TypeCode::Float32, AssignToAnyValueScalar<mvvm::float32>},
      {sup::dto::TypeCode::Float64, AssignToAnyValueScalar<mvvm::float64>},
      {sup::dto::TypeCode::String, AssignToAnyValueScalar<std::string>}};

  const ::sup::dto::TypeCode type_code = GetTypeCode(item.GetAnyTypeName());
  auto iter = kAssignToAnyValueScalarMap.find(type_code);
  if (iter == kAssignToAnyValueScalarMap.end())
  {
    throw std::runtime_error("Not a known scalar type code");
  }

  sup::dto::AnyValue result((::sup::dto::AnyType(type_code)));
  auto assign_function = iter->second;
  assign_function(item.Data(), result);

  return result;
}

}  // namespace sup::gui
