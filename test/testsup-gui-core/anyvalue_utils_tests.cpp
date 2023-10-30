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

#include "sup/gui/model/anyvalue_utils.h"

#include <sup/gui/core/exceptions.h>

#include <sup/dto/anytype_helper.h>
#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

using namespace sup::gui;

//! Testing methods from AnyValueUtils.

class AnyValueUtilsTest : public ::testing::Test
{
public:
};

//! Importing xml Procedure containing a single instruction.

TEST_F(AnyValueUtilsTest, GetJsonString)
{
  sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type, 42};
  EXPECT_EQ(ValuesToJSONString(anyvalue), "42");
}

//! Validating method AnyTypeFromJSONString.

TEST_F(AnyValueUtilsTest, AnyTypeFromJSONString)
{
  {  // malformed type (missed closing bracket)
    EXPECT_THROW(AnyTypeFromJSONString(R"RAW({"type":"int32")RAW"), RuntimeException);
  }

  {  // scalar
    sup::dto::AnyType expected_anytype(sup::dto::SignedInteger32Type);
    EXPECT_EQ(AnyTypeFromJSONString(R"RAW({"type":"int32"})RAW"), expected_anytype);
  }

  {  // struct with a single field
    sup::dto::AnyType expected_anytype = {{{"signed", {sup::dto::SignedInteger32Type}}},
                                          "structname"};
    std::string json_str(
        R"RAW({"type":"structname","attributes":[{"signed":{"type":"int32"}}]})RAW");
    EXPECT_EQ(AnyTypeFromJSONString(json_str), expected_anytype);
  }
}

//! Validating method AnyValueFromJSONString.

TEST_F(AnyValueUtilsTest, AnyValueFromTypeAndJSONValue)
{
  sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 42};

  {  // scalar
    std::string json_value("42");
    EXPECT_EQ(AnyValueFromJSONString(expected_anyvalue.GetType(), json_value), expected_anyvalue);
  }

  {  // malformed scalar
    std::string json_value("abc");
    EXPECT_THROW(AnyValueFromJSONString(expected_anyvalue.GetType(), json_value), RuntimeException);
  }
}

//! Validating method AnyValueFromJSONString.

TEST_F(AnyValueUtilsTest, AnyValueFromTypeAndJSONValueString)
{
  {  // scalar
    sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 42};
    std::string json_type(R"RAW({"type":"int32"})RAW");
    std::string json_value("42");
    EXPECT_EQ(AnyValueFromJSONString(json_type, json_value), expected_anyvalue);
  }

  {  // malformed scalar
    sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 42};
    std::string json_type(R"RAW({"type":"int32"})RAW");
    std::string json_value("abc");
    EXPECT_THROW(AnyValueFromJSONString(json_type, json_value), RuntimeException);
  }
}

//! Validating method AnyValueFromJSONString.

TEST_F(AnyValueUtilsTest, AnyValueFromJSONString)
{
  {  // scalar
    sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 42};
    std::string json_value(
        R"RAW([{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"int32"}},{"instance":42}])RAW");
    EXPECT_EQ(AnyValueFromJSONString(json_value), expected_anyvalue);
  }

  {  // malformed scalar
    std::string json_value(
        R"RAW([{"encoding":"sup-dto/v1.0/JSON"},{"datatype":{"type":"int32"}},{"instance":"abc"}])RAW");
    EXPECT_THROW(AnyValueFromJSONString(json_value), RuntimeException);
  }
}
