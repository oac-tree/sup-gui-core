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

#include "sup/gui/plotting/waveform_helper.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <sup/dto/anytype.h>

#include <gtest/gtest.h>

#include <stdexcept>

using namespace sup::gui;


//! Testing helper methods from waveform_helper.h

class WaveformHelperTest : public ::testing::Test
{
};

TEST_F(WaveformHelperTest, CreatePoint)
{
  auto point = CreatePoint(1, 2);
  EXPECT_TRUE(point->IsStruct());

  auto fields = point->GetChildren();
  ASSERT_EQ(fields.size(), 2);

  EXPECT_EQ(fields.at(0)->GetDisplayName(), kXFieldName);
  EXPECT_EQ(fields.at(1)->GetDisplayName(), kYFieldName);

  EXPECT_EQ(fields.at(0)->GetAnyTypeName(), sup::dto::kFloat64TypeName);
  EXPECT_EQ(fields.at(1)->GetAnyTypeName(), sup::dto::kFloat64TypeName);

  EXPECT_EQ(fields.at(0)->Data<double>(), 1.0);
  EXPECT_EQ(fields.at(1)->Data<double>(), 2.0);
}

TEST_F(WaveformHelperTest, CreatePlotData)
{
  auto plot_data = CreatePlotData({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});
  EXPECT_TRUE(plot_data->IsArray());

  auto structs = plot_data->GetChildren();
  ASSERT_EQ(structs.size(), 3);

  auto fields = structs.at(1)->GetChildren();  // middle point

  EXPECT_EQ(fields.at(0)->GetAnyTypeName(), sup::dto::kFloat64TypeName);
  EXPECT_EQ(fields.at(1)->GetAnyTypeName(), sup::dto::kFloat64TypeName);

  EXPECT_EQ(fields.at(0)->Data<double>(), 2.0);
  EXPECT_EQ(fields.at(1)->Data<double>(), 20.0);
}

TEST_F(WaveformHelperTest, GetXY)
{
  sup::gui::AnyValueItem item;
  EXPECT_THROW(GetXY(item), std::runtime_error);

  auto point = CreatePoint(1, 2);
  EXPECT_TRUE(point->IsStruct());

  EXPECT_EQ(GetXY(*point), std::make_pair(1.0, 2.0));
}

//! Testing method CreatePointToAppend.

TEST_F(WaveformHelperTest, CreatePointToAppend)
{
  {  // array without points
    sup::gui::AnyValueArrayItem array_item;

    auto new_point = CreatePointToAppend(array_item, nullptr);

    // point to append should be (0.0, 0.0)
    EXPECT_EQ(GetXY(*new_point), std::make_pair(0.0, 0.0));
  }

  {  // array with single point (1.0, 2.0)
    const double x{1.0};
    const double y{2.0};
    auto array_item = CreatePlotData({{x, y}});

    auto new_point = CreatePointToAppend(*array_item, nullptr);

    // point to append should be shifted (1.1, 2.0)
    auto [new_point_x, new_point_y] = GetXY(*new_point);
    EXPECT_DOUBLE_EQ(new_point_x, x + kDefaultDx);
    EXPECT_EQ(new_point_y, y);
  }

  {  // array with 3 points when middle is selected
    const double x_middle{2.0};
    const double y_middle{20.0};

    auto array_item = CreatePlotData({{1.0, 10.0}, {x_middle, y_middle}, {3.0, 30.0}});
    auto points = array_item->GetChildren();

    auto new_point = CreatePointToAppend(*array_item, points.at(1));

    // point to append should be shifted (2.1, 20.0)
    auto [new_point_x, new_point_y] = GetXY(*new_point);
    EXPECT_DOUBLE_EQ(new_point_x, x_middle + kDefaultDx);
    EXPECT_EQ(new_point_y, y_middle);
  }
}

//! Testing method CreatePointToAppend.

TEST_F(WaveformHelperTest, CreatePointToPrepend)
{
  {  // array without points
    sup::gui::AnyValueArrayItem array_item;

    auto new_point = CreatePointToPrepend(array_item, nullptr);

    // point to append should be (0.0, 0.0)
    EXPECT_EQ(GetXY(*new_point), std::make_pair(0.0, 0.0));
  }

  {  // array with single point (1.0, 2.0)
    const double x{1.0};
    const double y{2.0};
    auto array_item = CreatePlotData({{x, y}});

    auto new_point = CreatePointToPrepend(*array_item, nullptr);

    // point to append should be shifted (0.9, 2.0)
    auto [new_point_x, new_point_y] = GetXY(*new_point);
    EXPECT_DOUBLE_EQ(new_point_x, x - kDefaultDx);
    EXPECT_EQ(new_point_y, y);
  }

  {  // array with 3 points when middle is selected
    const double x_middle{2.0};
    const double y_middle{20.0};

    auto array_item = CreatePlotData({{1.0, 10.0}, {x_middle, y_middle}, {3.0, 30.0}});
    auto points = array_item->GetChildren();

    auto new_point = CreatePointToPrepend(*array_item, points.at(1));

    // point to append should be shifted (1.9, 20.0)
    auto [new_point_x, new_point_y] = GetXY(*new_point);
    EXPECT_DOUBLE_EQ(new_point_x, x_middle - kDefaultDx);
    EXPECT_EQ(new_point_y, y_middle);
  }
}

TEST_F(WaveformHelperTest, GetPoints)
{
  const std::vector<std::pair<double, double>> expected = {{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}};
  auto array_item = CreatePlotData(expected);

  EXPECT_EQ(GetPoints(array_item.get()), expected);
}

TEST_F(WaveformHelperTest, GetXYValues)
{
  auto array_item = CreatePlotData({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});

  EXPECT_EQ(GetXValues(array_item.get()), std::vector<double>({1.0, 2.0, 3.0}));
  EXPECT_EQ(GetYValues(array_item.get()), std::vector<double>({10.0, 20.0, 30.0}));
}
