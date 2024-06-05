/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "sup/gui/core/query_result.h"

#include <gtest/gtest.h>

using namespace sup::gui;

class QueryResultTest : public ::testing::Test
{
};

TEST_F(QueryResultTest, Success)
{
  auto querry = QueryResult::Success();
  EXPECT_TRUE(querry.IsSuccess());
}

TEST_F(QueryResultTest, Failure)
{
  auto querry = QueryResult::Failure({"title", "text", "informative", "detailed"});
  EXPECT_FALSE(querry.IsSuccess());
  EXPECT_EQ(querry.GetMessage().title, std::string("title"));
  EXPECT_EQ(querry.GetMessage().text, std::string("text"));
  EXPECT_EQ(querry.GetMessage().informative, std::string("informative"));
  EXPECT_EQ(querry.GetMessage().detailed, std::string("detailed"));
}
