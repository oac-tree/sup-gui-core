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

#include "anyvalue_utils.h"

#include <sup/dto/anytype_helper.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/json_type_parser.h>
#include <sup/dto/json_value_parser.h>

#include <map>
#include <sstream>
#include <stdexcept>

namespace
{
// FIXME the code is borrowed from CLIInterface.cpp
// Remove duplication

using ParseFunction = bool (*)(sup::dto::AnyValue &, const std::string &);

template <typename T>
bool ParserFunctionT(sup::dto::AnyValue &value, const std::string &str)
{
  std::istringstream istr(str);
  T val;
  istr >> val;
  if (istr.fail())
  {
    throw std::runtime_error("ParseStringToScalarAnyvalue() - could not parse");
  }
  value = val;
  return true;
}

/**
 * @todo Extend possible input values: currently only 'true' or 'false'. Possible alternatives
 * are integers (zero is false), case insensitive true/false, yes/no, etc.
 */
template <>
bool ParserFunctionT<sup::dto::boolean>(sup::dto::AnyValue &value, const std::string &str)
{
  std::istringstream istr(str);
  sup::dto::boolean val;
  istr >> std::boolalpha >> val;
  if (istr.fail())
  {
    throw std::runtime_error("ParseStringToScalarAnyvalue() - could not parse");
  }
  value = val;
  return true;
}

template <>
bool ParserFunctionT<std::string>(sup::dto::AnyValue &value, const std::string &str)
{
  value = str;
  return true;
}

static std::map<std::string, ParseFunction> CreateParserMap()
{
  std::map<std::string, ParseFunction> parser_map;
  parser_map["bool"] = ParserFunctionT<sup::dto::boolean>;
  parser_map["char8"] = ParserFunctionT<sup::dto::char8>;
  parser_map["int8"] = ParserFunctionT<sup::dto::int8>;
  parser_map["uint8"] = ParserFunctionT<sup::dto::uint8>;
  parser_map["int16"] = ParserFunctionT<sup::dto::int16>;
  parser_map["uint16"] = ParserFunctionT<sup::dto::uint16>;
  parser_map["int32"] = ParserFunctionT<sup::dto::int32>;
  parser_map["uint32"] = ParserFunctionT<sup::dto::uint32>;
  parser_map["uint64"] = ParserFunctionT<sup::dto::uint64>;
  parser_map["float32"] = ParserFunctionT<sup::dto::float32>;
  parser_map["float64"] = ParserFunctionT<sup::dto::float64>;
  parser_map["string"] = ParserFunctionT<std::string>;
  return parser_map;
}

static std::map<std::string, ParseFunction> &GetParserMap()
{
  static std::map<std::string, ParseFunction> parser_map = CreateParserMap();
  return parser_map;
}

}  // namespace

namespace sup::gui
{

std::string AnyValueToJSONString(const anyvalue_t &value, bool is_pretty)
{
  return sup::dto::AnyValueToJSONString(value, is_pretty);
}

std::string AnyTypeToJSONString(const anyvalue_t &value)
{
  return sup::dto::AnyTypeToJSONString(value.GetType());
}

std::string ValuesToJSONString(const anyvalue_t &value)
{
  return sup::dto::ValuesToJSONString(value);
}

bool ParseStringToScalarAnyvalue(const std::string &str, sup::dto::AnyValue &value)
{
  std::string type_name = value.GetTypeName();

  auto &parser_map = GetParserMap();
  if (parser_map.find(type_name) == parser_map.end())
  {
    return false;
  }
  auto parse_function = parser_map[type_name];
  return parse_function(value, str);
}

sup::dto::AnyValue AnyValueFromJSONFile(const std::string &filename)
{
  sup::dto::JSONAnyValueParser parser;
  parser.ParseFile(filename);
  return parser.MoveAnyValue();
}

sup::dto::AnyType AnyTypeFromJSONString(const std::string &str)
{
  sup::dto::JSONAnyTypeParser parser;
  if (!parser.ParseString(str))
  {
    throw std::runtime_error("Can't parse Json type from string '" + str + "'");
  }
  return parser.MoveAnyType();
}

dto::AnyValue AnyValueFromJSONString(const std::string &json_type, const std::string &json_value)
{
  auto anytype = AnyTypeFromJSONString(json_type);

  sup::dto::JSONAnyValueParser value_parser;
  if (!value_parser.TypedParseString(anytype, json_value))
  {
    throw std::runtime_error("Can't parse Json value from type '" + json_type
                             + "' and value string '" + json_value + "'");
  }

  return value_parser.MoveAnyValue();
}

}  // namespace sup::gui
