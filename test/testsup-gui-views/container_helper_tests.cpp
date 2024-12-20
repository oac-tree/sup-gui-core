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

#include "sup/gui/widgets/container_helper.h"

#include <gtest/gtest.h>

namespace sup::gui
{

class ContainerHelperTest : public ::testing::Test
{
};

TEST_F(ContainerHelperTest, ListToByteArray)
{
  QList<int> list({1, 2, 3});
  auto byte_array = GetArray(list);

  EXPECT_EQ(GetList(byte_array), list);
}

}  // namespace sup::gui
