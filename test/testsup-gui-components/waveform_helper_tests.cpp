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

#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/point_item.h>

#include <sup/dto/anytype.h>

#include <gtest/gtest.h>

#include <stdexcept>

using namespace sup::gui;

/**
 * @brief Testing functions from waveform_helper.h
 */
class WaveformHelperTest : public ::testing::Test
{
};

TEST_F(WaveformHelperTest, CreateFromPoint)
{
  auto point = CreateFromPoint(1, 2);
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

TEST_F(WaveformHelperTest, CreateFromWaveform)
{
  auto plot_data = CreateFromWaveform({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});
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
  sup::gui::AnyValueItem item("Base");
  EXPECT_THROW(GetPoint(item), std::runtime_error);

  auto point = CreateFromPoint(1, 2);
  EXPECT_TRUE(point->IsStruct());

  EXPECT_EQ(GetPoint(*point), std::make_pair(1.0, 2.0));
}

//! Testing method CreatePointToAppend.
TEST_F(WaveformHelperTest, CreatePointToAppend)
{
  {  // waveform without points
    mvvm::LineSeriesDataItem data_item;

    auto new_point = CreatePointToAppend(data_item, nullptr);

    // point to append should be (0.0, 0.0)
    EXPECT_EQ(new_point->GetPointCoordinates(), std::make_pair(0.0, 0.0));
  }

  {  // waveform with single point (1.0, 2.0)
    mvvm::LineSeriesDataItem data_item;

    const double x{1.0};
    const double y{2.0};
    data_item.SetWaveform({{x, y}});

    auto new_point = CreatePointToAppend(data_item, nullptr);

    // point to append should be shifted (1.1, 2.0)
    EXPECT_DOUBLE_EQ(new_point->GetX(), x + kDefaultDx);
    EXPECT_EQ(new_point->GetY(), y);
  }

  {  // waveform with 3 points when middle is selected
    mvvm::LineSeriesDataItem data_item;

    const double x_middle{2.0};
    const double y_middle{20.0};
    data_item.SetWaveform({{1.0, 10.0}, {x_middle, y_middle}, {3.0, 30.0}});

    auto new_point = CreatePointToAppend(data_item, data_item.GetPoint(1));

    // point to append should be shifted (2.1, 20.0)
    EXPECT_DOUBLE_EQ(new_point->GetX(), x_middle + kDefaultDx);
    EXPECT_EQ(new_point->GetY(), y_middle);
  }
}

//! Testing method CreatePointToAppend.
TEST_F(WaveformHelperTest, CreatePointToPrepend)
{
  {  // waveform without points
    mvvm::LineSeriesDataItem data_item;

    auto new_point = CreatePointToPrepend(data_item, nullptr);

    // point to append should be (0.0, 0.0)
    EXPECT_EQ(new_point->GetPointCoordinates(), std::make_pair(0.0, 0.0));
  }

  {  // waveform with single point (1.0, 2.0)
    mvvm::LineSeriesDataItem data_item;

    const double x{1.0};
    const double y{2.0};
    data_item.SetWaveform({{x, y}});

    auto new_point = CreatePointToPrepend(data_item, nullptr);

    // point to append should be shifted (0.9, 2.0)
    EXPECT_DOUBLE_EQ(new_point->GetX(), x - kDefaultDx);
    EXPECT_EQ(new_point->GetY(), y);
  }

  {  // array with 3 points when middle is selected
    const double x_middle{2.0};
    const double y_middle{20.0};

    mvvm::LineSeriesDataItem data_item;
    data_item.SetWaveform({{1.0, 10.0}, {x_middle, y_middle}, {3.0, 30.0}});

    auto new_point = CreatePointToPrepend(data_item, data_item.GetPoint(1));

    // point to append should be shifted (1.9, 20.0)
    EXPECT_DOUBLE_EQ(new_point->GetX(), x_middle - kDefaultDx);
    EXPECT_EQ(new_point->GetY(), y_middle);
  }
}

TEST_F(WaveformHelperTest, GetWaveform)
{
  const std::vector<std::pair<double, double>> expected = {{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}};
  auto array_item = CreateFromWaveform(expected);

  EXPECT_EQ(GetWaveform(array_item.get()), expected);
}

TEST_F(WaveformHelperTest, SetupNewWaveform)
{
  mvvm::LineSeriesItem item;

  SetupNewWaveform(item, 1);

  // hardcoded in SetupNewWaveform()
  EXPECT_EQ(item.GetDisplayName(), std::string("waveform1"));
  EXPECT_EQ(item.GetNamedColor(), std::string("green"));
}
