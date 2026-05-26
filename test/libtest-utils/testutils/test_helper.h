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

#ifndef TESTUTILS_TEST_HELPER_H_
#define TESTUTILS_TEST_HELPER_H_

#include <string>

namespace sup::dto
{
class AnyValue;
}

namespace sup::gui::test
{

/**
 * @brief Creates test file with serialized content
 */
void CreateSerializedFile(const sup::dto::AnyValue& value, const std::string& file_name);

}

#endif // TESTUTILS_TEST_HELPER_H_
