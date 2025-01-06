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

sup::dto::AnyValue AnyValueFromJSONFile(const std::string &filename,
                                        const anytype_registry_t *registry)
{
  sup::dto::JSONAnyValueParser parser;
  parser.ParseFile(filename, registry);
  return parser.MoveAnyValue();
}

sup::dto::AnyType AnyTypeFromJSONString(const std::string &str, const anytype_registry_t *registry)
{
  sup::dto::JSONAnyTypeParser parser;
  if (!parser.ParseString(str, registry))
  {
    throw RuntimeException("Can't parse Json type from string '" + str + "'");
  }
  return parser.MoveAnyType();
}

sup::dto::AnyValue AnyValueFromJSONString(const std::string &str,
                                          const anytype_registry_t *registry)
{
  sup::dto::JSONAnyValueParser parser;
  if (!parser.ParseString(str, registry))
  {
    throw RuntimeException("Can't parse Json value from string '" + str + "'");
  }
  return parser.MoveAnyValue();
}

dto::AnyValue AnyValueFromJSONString(const dto::AnyType &anytype, const std::string &value_str)
{
  sup::dto::JSONAnyValueParser value_parser;
  if (!value_parser.TypedParseString(anytype, value_str))
  {
    throw RuntimeException("Can't parse Json value from value '" + value_str + "'");
  }

  return value_parser.MoveAnyValue();
}

dto::AnyValue AnyValueFromJSONString(const std::string &type_str, const std::string &value_str)
{
  auto anytype = AnyTypeFromJSONString(type_str);
  return AnyValueFromJSONString(anytype, value_str);
}

}  // namespace sup::gui
