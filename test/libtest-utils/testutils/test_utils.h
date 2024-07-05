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

#ifndef TESTUTILS_TEST_UTILS_H_
#define TESTUTILS_TEST_UTILS_H_

#include <string>

namespace testutils
{
//! Returns full path to the main test folder, as defined by CMake at compile time.
//! Shoud point to CMAKE_BINARY_DIR/test_output
std::string GetTestSuiteOutputDir();

//! Returns true if given item can be casted to desired type.
template <typename DesiredT, typename ItemT>
bool CanCast(const ItemT* item)
{
  return dynamic_cast<const DesiredT*>(item) != nullptr;
}

}  // namespace testutils

#endif  // TESTUTILS_TEST_UTILS_H_
