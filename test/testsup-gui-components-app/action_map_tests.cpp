/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "sup/gui/components/action_map.h"

#include <gtest/gtest.h>

using namespace sup::gui;

class ActionMapTest : public ::testing::Test
{
public:
  enum class Keys
  {
    Key1,
    Key2,
    Key3
  };
};

TEST_F(ActionMapTest, AddAction)
{
  ActionMap<Keys> action_map;
  EXPECT_EQ(action_map.GetSize(), 0);

  QAction action1;
  QAction action2;

  // it is not possible to register null action
  EXPECT_THROW(action_map.Add(Keys::Key1, nullptr), RuntimeException);

  action_map.Add(Keys::Key1, &action1);
  EXPECT_EQ(action_map.GetSize(), 1);

  // it is not possible to register key twice
  EXPECT_THROW(action_map.Add(Keys::Key1, &action1), RuntimeException);

  action_map.Add(Keys::Key2, &action2);

  EXPECT_EQ(action_map.GetActions({Keys::Key2, Keys::Key1}), QList<QAction*>({&action2, &action1}));
}

TEST_F(ActionMapTest, GetKey)
{
  ActionMap<Keys> action_map;
  EXPECT_EQ(action_map.GetSize(), 0);

  QAction action1;
  QAction action2;

  action_map.Add(Keys::Key1, &action1);
  action_map.Add(Keys::Key2, &action2);

  EXPECT_EQ(action_map.GetKey(&action1), Keys::Key1);
  EXPECT_EQ(action_map.GetKey(&action2), Keys::Key2);
  EXPECT_THROW(action_map.GetKey(nullptr), RuntimeException);
}
