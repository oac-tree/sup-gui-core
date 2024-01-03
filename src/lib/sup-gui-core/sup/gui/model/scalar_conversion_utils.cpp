/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <sup/gui/core/exceptions.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/basic_scalar_types.h>

#include <functional>
#include <map>

namespace
{

/**
 * @brief Assigns scalar value from variant_t to pre-created AnyValue value.
 */
template <typename T>
void AssignToAnyValueScalar(const mvvm::variant_t &variant, sup::dto::AnyValue &any_value)
{
  any_value.ConvertFrom(std::get<T>(variant));
}

/**
 * @brief Creates variant from scalar-like AnyValue.
 */
template <typename T>
mvvm::variant_t GetVariantFromScalarT(const sup::dto::AnyValue &anyvalue)
{
  T val = anyvalue.As<T>();
  return {val};
}

using sup::dto::TypeCode;
using function_t = std::function<mvvm::variant_t(const sup::dto::AnyValue &anyvalue)>;

const std::map<TypeCode, function_t> kConversionMap{
    {TypeCode::Bool, GetVariantFromScalarT<sup::dto::boolean>},
    {TypeCode::Char8, GetVariantFromScalarT<sup::dto::char8>},
    {TypeCode::Int8, GetVariantFromScalarT<sup::dto::int8>},
    {TypeCode::UInt8, GetVariantFromScalarT<sup::dto::uint8>},
    {TypeCode::Int16, GetVariantFromScalarT<sup::dto::int16>},
    {TypeCode::UInt16, GetVariantFromScalarT<sup::dto::uint16>},
    {TypeCode::Int32, GetVariantFromScalarT<sup::dto::int32>},
    {TypeCode::UInt32, GetVariantFromScalarT<sup::dto::uint32>},
    {TypeCode::Int64, GetVariantFromScalarT<sup::dto::int64>},
    {TypeCode::UInt64, GetVariantFromScalarT<sup::dto::uint64>},
    {TypeCode::Float32, GetVariantFromScalarT<sup::dto::float32>},
    {TypeCode::Float64, GetVariantFromScalarT<sup::dto::float64>},
    {TypeCode::String, GetVariantFromScalarT<std::string>}};

}  // namespace

namespace sup::gui
{

mvvm::variant_t GetVariantFromScalar(const anyvalue_t &value)
{
  auto iter = kConversionMap.find(value.GetTypeCode());
  if (iter == kConversionMap.end())
  {
    throw RuntimeException("Not a known scalar type code");
  }
  return iter->second(value);
}

dto::AnyValue GetAnyValueFromScalar(const mvvm::variant_t &variant)
{
  using anyvalue_function_t =
      std::function<void(const mvvm::variant_t &variant, sup::dto::AnyValue &anyvalue)>;

  //! Correspondance of AnyValue type code to PVXS value function to assign scalars.
  static const std::map<sup::dto::TypeCode, anyvalue_function_t> kConversionMap = {
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

  const ::sup::dto::TypeCode type_code = GetScalarTypeCode(mvvm::utils::TypeName(variant));

  auto iter = kConversionMap.find(type_code);
  if (iter == kConversionMap.end())
  {
    throw RuntimeException("Not a known scalar type code");
  }

  sup::dto::AnyValue result((::sup::dto::AnyType(type_code)));
  auto assign_function = iter->second;
  assign_function(variant, result);

  return result;
}

mvvm::variant_t GetVariantFromScalarTypeName(const std::string &type_name)
{
  sup::dto::AnyValue anyvalue(sup::dto::AnyType(GetScalarTypeCode(type_name)));
  return GetVariantFromScalar(anyvalue);
}

}  // namespace sup::gui
