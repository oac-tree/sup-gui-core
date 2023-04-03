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

#include <sup/gui/core/exceptions.h>

#include <sup/dto/anytype_helper.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/json_type_parser.h>
#include <sup/dto/json_value_parser.h>

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
    throw RuntimeException("Can't parse Json type from string '" + str + "'");
  }
  return parser.MoveAnyType();
}

dto::AnyValue AnyValueFromJSONString(const std::string &json_type, const std::string &json_value)
{
  auto anytype = AnyTypeFromJSONString(json_type);

  sup::dto::JSONAnyValueParser value_parser;
  if (!value_parser.TypedParseString(anytype, json_value))
  {
    throw RuntimeException("Can't parse Json value from type '" + json_type + "' and value string '"
                           + json_value + "'");
  }

  return value_parser.MoveAnyValue();
}

}  // namespace sup::gui
