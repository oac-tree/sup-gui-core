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

#include "sup/gui/plotting/chart_items.h"

#include <sup/gui/plotting/waveform_helper.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/axis_items.h>

#include <gtest/gtest.h>

using namespace sup::gui;

class ChartItemsTest : public ::testing::Test
{
};

TEST_F(ChartItemsTest, LineSeriesItemInitialState)
{
  const LineSeriesItem item;
  EXPECT_TRUE(item.GetDataItem() == nullptr);
  EXPECT_EQ(item.GetBinCenters(), std::vector<double>{});
  EXPECT_EQ(item.GetValues(), std::vector<double>{});
  EXPECT_EQ(item.GetNamedColor(), kDefaultLineColor);
}

TEST_F(ChartItemsTest, LineSeriesItemSetNamedColor)
{
  LineSeriesItem item;
  item.SetNamedColor("mediumaquamarine");
  EXPECT_EQ(item.GetNamedColor(), std::string("mediumaquamarine"));
}

//! Validating link mechanism.

TEST_F(ChartItemsTest, LineSeriesItemSetData)
{
  mvvm::ApplicationModel model;

  // creating data item, and linking series item to it
  auto data_item = model.InsertItem<AnyValueArrayItem>();
  auto line_series_item = model.InsertItem<LineSeriesItem>();
  line_series_item->SetDataItem(data_item);

  // checking that link is working
  EXPECT_EQ(line_series_item->GetDataItem(), data_item);
}

TEST_F(ChartItemsTest, BinCentersAndValues)
{
  mvvm::ApplicationModel model;
  auto line_series_item = model.InsertItem<LineSeriesItem>();

  // adding points to the model, make LineSeriesLooking at it
  auto array_item = CreatePlotData({{1.0, 10.0}, {2.0, 25.0}, {3.0, 10.0}});
  auto array_item_ptr = array_item.get();
  model.InsertItem(std::move(array_item), model.GetRootItem(), mvvm::TagIndex::Append());
  line_series_item->SetDataItem(array_item_ptr);

  EXPECT_EQ(line_series_item->GetBinCenters(), std::vector<double>({1.0, 2.0, 3.0}));
  EXPECT_EQ(line_series_item->GetValues(), std::vector<double>({10.0, 25.0, 10.0}));
}

TEST_F(ChartItemsTest, BinCentersAndValuesWhenOffsetIsPresent)
{
  mvvm::ApplicationModel model;
  auto line_series_item = model.InsertItem<LineSeriesItem>();
  line_series_item->SetXOffset(10.0);

  // adding points to the model, make LineSeriesLooking at it
  auto array_item = CreatePlotData({{1.0, 10.0}, {2.0, 25.0}, {3.0, 10.0}});
  auto array_item_ptr = array_item.get();
  model.InsertItem(std::move(array_item), model.GetRootItem(), mvvm::TagIndex::Append());
  line_series_item->SetDataItem(array_item_ptr);

  EXPECT_EQ(line_series_item->GetBinCenters(), std::vector<double>({11.0, 12.0, 13.0}));
  EXPECT_EQ(line_series_item->GetValues(), std::vector<double>({10.0, 25.0, 10.0}));
}

TEST_F(ChartItemsTest, SetOffset)
{
  LineSeriesItem item;
  EXPECT_EQ(item.GetXOffset(), 0.0);

  item.SetXOffset(42.0);
  EXPECT_EQ(item.GetXOffset(), 42.0);
}

TEST_F(ChartItemsTest, ChartViewportItem)
{
  mvvm::ApplicationModel model;

  auto viewport_item = model.InsertItem<ChartViewportItem>();
  auto line_series_item = model.InsertItem<LineSeriesItem>(viewport_item);

  EXPECT_NE(viewport_item->GetXAxis(), nullptr);
  EXPECT_NE(viewport_item->GetYAxis(), nullptr);

  EXPECT_EQ(viewport_item->GetXAxis()->GetMin(), 0.0);
  EXPECT_EQ(viewport_item->GetXAxis()->GetMax(), 1.0);
  EXPECT_EQ(viewport_item->GetYAxis()->GetMin(), 0.0);
  EXPECT_EQ(viewport_item->GetYAxis()->GetMax(), 1.0);

  EXPECT_EQ(viewport_item->GetLineSeries(), std::vector<LineSeriesItem*>({line_series_item}));
}

TEST_F(ChartItemsTest, SetViewportToContent)
{
  mvvm::ApplicationModel model;

  auto viewport_item = model.InsertItem<ChartViewportItem>();
  auto line_series_item = model.InsertItem<LineSeriesItem>(viewport_item);

  // setting viewport to content when viewport is empty
  viewport_item->SetViewportToContent();
  EXPECT_EQ(viewport_item->GetXAxis()->GetMin(), 0.0);
  EXPECT_EQ(viewport_item->GetXAxis()->GetMax(), 1.0);
  EXPECT_EQ(viewport_item->GetYAxis()->GetMin(), 0.0);
  EXPECT_EQ(viewport_item->GetYAxis()->GetMax(), 1.0);

  // adding points to the model, make LineSeriesLooking at it
  auto array_item0 = CreatePlotData({{1.0, 10.0}, {2.0, 25.0}, {3.0, 10.0}});
  auto array_item0_ptr = array_item0.get();
  model.InsertItem(std::move(array_item0), model.GetRootItem(), mvvm::TagIndex::Append());
  line_series_item->SetDataItem(array_item0_ptr);

  // setting viewport to content
  viewport_item->SetViewportToContent();

  EXPECT_EQ(viewport_item->GetXAxis()->GetMin(), 1.0);
  EXPECT_EQ(viewport_item->GetXAxis()->GetMax(), 3.0);
  EXPECT_EQ(viewport_item->GetYAxis()->GetMin(), 10.0);
  EXPECT_EQ(viewport_item->GetYAxis()->GetMax(), 25.0);
}

TEST_F(ChartItemsTest, SetViewportToContentTwoLineSeriesWithOffset)
{
  mvvm::ApplicationModel model;

  // creating viewport
  auto viewport_item = model.InsertItem<ChartViewportItem>();

  // adding first line series
  auto line_series_item0 = model.InsertItem<LineSeriesItem>(viewport_item);
  line_series_item0->SetXOffset(10.0);
  // setting data to it
  auto array_item0 = CreatePlotData({{1.0, 10.0}, {2.0, 20.0}});
  auto array_item_ptr0 = array_item0.get();
  model.InsertItem(std::move(array_item0), model.GetRootItem(), mvvm::TagIndex::Append());
  line_series_item0->SetDataItem(array_item_ptr0);

  // adding second line series
  auto line_series_item1 = model.InsertItem<LineSeriesItem>(viewport_item);
  line_series_item1->SetXOffset(20.0);
  // setting data to it
  auto array_item1 = CreatePlotData({{1.0, 12.0}, {3.0, 30.0}});
  auto array_item_ptr1 = array_item1.get();
  model.InsertItem(std::move(array_item1), model.GetRootItem(), mvvm::TagIndex::Append());
  line_series_item1->SetDataItem(array_item_ptr1);

  // setting viewport to content
  viewport_item->SetViewportToContent();

  // x-axis of viewport should be set to xmin, xmax between two graphs, with offset taken into
  // accoun
  auto xaxis = viewport_item->GetXAxis();
  EXPECT_DOUBLE_EQ(xaxis->GetMin(), 11.0);  // xmin of first item (+offset 10)
  EXPECT_DOUBLE_EQ(xaxis->GetMax(), 23.0);  // xmax of second item  (offset 20)

  // y-axis of viewport should be set to xmin, xmax between two graphs, with offset taken into
  // accoun
  auto yaxis = viewport_item->GetYAxis();
  EXPECT_DOUBLE_EQ(yaxis->GetMin(), 10.0);  // ymin of first item
  EXPECT_DOUBLE_EQ(yaxis->GetMax(), 30.0);  // ymax of second item
}
