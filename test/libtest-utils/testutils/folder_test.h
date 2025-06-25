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

#ifndef TESTUTILS_FOLDER_TEST_H_
#define TESTUTILS_FOLDER_TEST_H_

#include <mvvm/test/folder_output_based_test.h>

namespace sup::gui::test
{

class FolderTest : public mvvm::test::FolderOutputBasedTest
{
public:
  explicit FolderTest(std::string test_home_dirname);
};

}  // namespace sup::gui::test

#endif  // TESTUTILS_FOLDER_TEST_H_
