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

#ifndef TESTUTILS_FOLDER_TEST_H_
#define TESTUTILS_FOLDER_TEST_H_

#include <mvvm/test/folder_output_based_test.h>

namespace testutils
{

class FolderTest : public mvvm::test::FolderOutputBasedTest
{
public:
  explicit FolderTest(std::string test_home_dirname);
};

/**
 * @brief  Returns full path to the main test folder, as defined by CMake at compile time.
 *
 * Shoud point to CMAKE_BINARY_DIR/test_output
 */
std::string GetTestSuiteOutputDir();

/**
 * @brief Returns full path to repository source directory.
 * @return
 */
std::string GetProjectSourceDir();

}  // namespace testutils

#endif  // TESTUTILS_FOLDER_TEST_H_
