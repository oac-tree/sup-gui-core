/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "test_helper.h"

#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/test/test_helper.h>

namespace sup::gui::test
{

void CreateSerializedFile(const dto::AnyValue& value, const std::string& file_name)
{
  auto json_content = AnyValueToJSONString(value);
  mvvm::test::CreateTextFile(file_name, json_content);
}

}  // namespace sup::gui::test
