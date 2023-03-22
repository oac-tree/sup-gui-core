/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "sup/gui/model/anyvalue_utils.h"

#include <sup/dto/anytype_helper.h>
#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

//! Testing methods from ShallNotBeNamedValueUtils

class AnyValueUtilsTest : public ::testing::Test
{
public:
};

//! Importing xml Procedure containing a single instruction.

TEST_F(AnyValueUtilsTest, GetJsonString)
{
  sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type, 42};
  EXPECT_EQ(sup::gui::GetValuesToJSONString(&anyvalue), "42");
}

//! Validating method AnyTypeFromJSONString.

TEST_F(AnyValueUtilsTest, AnyTypeFromJSONString)
{
  {  // malformed type (missed closing bracket)
    EXPECT_THROW(sup::gui::AnyTypeFromJSONString(R"RAW({"type":"int32")RAW"), std::runtime_error);
  }

  {  // scalar
    sup::dto::AnyType expected_anytype(sup::dto::SignedInteger32Type);
    EXPECT_EQ(sup::gui::AnyTypeFromJSONString(R"RAW({"type":"int32"})RAW"), expected_anytype);
  }

  {  // struct with a single field
    sup::dto::AnyType expected_anytype = {{{"signed", {sup::dto::SignedInteger32Type}}},
                                          "structname"};
    std::string json_str(
        R"RAW({"type":"structname","attributes":[{"signed":{"type":"int32"}}]})RAW");
    EXPECT_EQ(sup::gui::AnyTypeFromJSONString(json_str), expected_anytype);
  }
}
