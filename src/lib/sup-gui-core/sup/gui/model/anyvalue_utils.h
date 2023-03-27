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

#ifndef SUP_GUI_MODEL_ANYVALUE_UTILS_H_
#define SUP_GUI_MODEL_ANYVALUE_UTILS_H_

//! Collection of sup::dto::AnyValue related utility functions.

#include <sup/gui/core/dto_types_fwd.h>

#include <string>

namespace sup::gui
{

std::string GetAnyValueToJSONString(const anyvalue_t& value, bool is_pretty = false);

std::string GetAnyTypeToJSONString(const anyvalue_t& value);

std::string GetValuesToJSONString(const anyvalue_t& value);

bool ParseStringToScalarAnyvalue(const std::string& str, anyvalue_t& value);

sup::dto::AnyValue AnyValueFromJSONFile(const std::string& filename);

sup::dto::AnyType AnyTypeFromJSONString(const std::string& str);

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_ANYVALUE_UTILS_H_
