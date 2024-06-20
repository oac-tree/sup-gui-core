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

#include "sup/gui/plotting/waveform_twocolumn_viewmodel.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/plotting/waveform_helper.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QTransposeProxyModel>

using namespace sup::gui;

class WaveformTwoColumnViewModelTest : public ::testing::Test
{
};

//! Testing how viewmodel looks like when pointed to time series of 3 (x,y) points.

TEST_F(WaveformTwoColumnViewModelTest, ViewModelData)
{
  mvvm::ApplicationModel model;

  // an item representing time series
  auto array_item = sup::gui::CreatePlotData({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});

  auto item =
      model.InsertItem(std::move(array_item), model.GetRootItem(), mvvm::TagIndex::Append());

  WaveformTwoColumnViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(item);

  EXPECT_EQ(viewmodel.rowCount(), 3);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  // first point
  auto x_index = viewmodel.index(0, 0);
  auto y_index = viewmodel.index(0, 1);
  EXPECT_EQ(viewmodel.data(x_index, Qt::DisplayRole).toDouble(), 1.0);
  EXPECT_EQ(viewmodel.data(y_index, Qt::DisplayRole).toDouble(), 10.0);

  // second point
  x_index = viewmodel.index(1, 0);
  y_index = viewmodel.index(1, 1);
  EXPECT_EQ(viewmodel.data(x_index, Qt::DisplayRole).toDouble(), 2.0);
  EXPECT_EQ(viewmodel.data(y_index, Qt::DisplayRole).toDouble(), 20.0);

  // third point
  x_index = viewmodel.index(2, 0);
  y_index = viewmodel.index(2, 1);
  EXPECT_EQ(viewmodel.data(x_index, Qt::DisplayRole).toDouble(), 3.0);
  EXPECT_EQ(viewmodel.data(y_index, Qt::DisplayRole).toDouble(), 30.0);
}

//! Testing how viewmodel looks like when pointed to time series of 3 (x,y) points.
//! This time we are testing special QTransposeProxyModel and expectind that
//! nrow=3, ncol=2 will become nrow=2, ncol=3.

TEST_F(WaveformTwoColumnViewModelTest, TwoColumnModelTransposedProxy)
{
  // 1.0, 10.0
  // 2.0, 20.0
  // 3.0, 30.0
  //
  // should become
  // 1.0  2.0  3.0
  // 10.0 20.0 30.0

  mvvm::ApplicationModel model;

  // an item representing time series
  auto array_item = sup::gui::CreatePlotData({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});
  const auto struct_items = array_item->GetChildren();
  ASSERT_EQ(struct_items.size(), 3);

  auto item =
      model.InsertItem(std::move(array_item), model.GetRootItem(), mvvm::TagIndex::Append());

  WaveformTwoColumnViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(item);

  QTransposeProxyModel proxy;
  proxy.setSourceModel(&viewmodel);

  EXPECT_EQ(proxy.rowCount(), 2);
  EXPECT_EQ(proxy.columnCount(), 3);

  // first row
  auto x_col0 = proxy.index(0, 0);
  auto x_col1 = proxy.index(0, 1);
  auto x_col2 = proxy.index(0, 2);
  EXPECT_EQ(proxy.data(x_col0, Qt::DisplayRole).toDouble(), 1.0);
  EXPECT_EQ(proxy.data(x_col1, Qt::DisplayRole).toDouble(), 2.0);
  EXPECT_EQ(proxy.data(x_col2, Qt::DisplayRole).toDouble(), 3.0);

  // second row
  auto y_col0 = proxy.index(1, 0);
  auto y_col1 = proxy.index(1, 1);
  auto y_col2 = proxy.index(1, 2);
  EXPECT_EQ(proxy.data(y_col0, Qt::DisplayRole).toDouble(), 10.0);
  EXPECT_EQ(proxy.data(y_col1, Qt::DisplayRole).toDouble(), 20.0);
  EXPECT_EQ(proxy.data(y_col2, Qt::DisplayRole).toDouble(), 30.0);

  // modifying y-value of the second colum via proxy
  proxy.setData(y_col1, 42.0);

  // validating via original AnyValueStructItem
  auto scalar_fields = struct_items.at(1)->GetChildren();
  ASSERT_EQ(scalar_fields.size(), 2);
  EXPECT_EQ(scalar_fields.at(1)->Data<double>(), 42.0);
}

//! Validating viewmodel while populating the model with time series points.

TEST_F(WaveformTwoColumnViewModelTest, AppendPoints)
{
  mvvm::ApplicationModel model;

  WaveformTwoColumnViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 0);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  auto array_item = model.InsertItem<sup::gui::AnyValueArrayItem>();
  viewmodel.SetRootSessionItem(array_item);
  EXPECT_EQ(viewmodel.rowCount(), 0);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  // adding a point to the array
  model.InsertItem(sup::gui::CreatePoint(1, 10.0), array_item, mvvm::TagIndex::Append());
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 2);
}

//! Validating viewmodel while removing points from the model.

TEST_F(WaveformTwoColumnViewModelTest, RemovePoints)
{
  mvvm::ApplicationModel model;

  // an item representing time series
  auto array_item = sup::gui::CreatePlotData({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});
  const auto struct_items = array_item->GetChildren();

  auto item =
      model.InsertItem(std::move(array_item), model.GetRootItem(), mvvm::TagIndex::Append());

  WaveformTwoColumnViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(item);

  EXPECT_EQ(viewmodel.rowCount(), 3);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  // removing middle point
  model.RemoveItem(struct_items.at(1));

  // viewmodel should see only first and last point
  EXPECT_EQ(viewmodel.rowCount(), 2);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  // first point
  auto x_index = viewmodel.index(0, 0);
  auto y_index = viewmodel.index(0, 1);
  EXPECT_EQ(viewmodel.data(x_index, Qt::DisplayRole).toDouble(), 1.0);
  EXPECT_EQ(viewmodel.data(y_index, Qt::DisplayRole).toDouble(), 10.0);

  // last point
  x_index = viewmodel.index(1, 0);
  y_index = viewmodel.index(1, 1);
  EXPECT_EQ(viewmodel.data(x_index, Qt::DisplayRole).toDouble(), 3.0);
  EXPECT_EQ(viewmodel.data(y_index, Qt::DisplayRole).toDouble(), 30.0);
}
