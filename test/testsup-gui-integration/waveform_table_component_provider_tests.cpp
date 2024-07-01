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

#include "sup/gui/plotting/waveform_table_component_provider.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/item_constants.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/point_item.h>

#include <gtest/gtest.h>

#include <QTableView>
#include <QTransposeProxyModel>

using namespace sup::gui;

class WaveformTableComponentProviderTest : public ::testing::Test
{
};

TEST_F(WaveformTableComponentProviderTest, InitialState)
{
  mvvm::ApplicationModel model;
  QTableView table_view;

  WaveformTableComponentProvider provider(&model, &table_view);

  EXPECT_NE(provider.GetViewModel(), nullptr);
  EXPECT_NE(provider.GetProxyModel(), nullptr);
  EXPECT_NE(provider.GetSelectionModel(), nullptr);

  ASSERT_NE(dynamic_cast<QTransposeProxyModel *>(table_view.model()), nullptr);
  EXPECT_EQ(provider.GetProxyModel(), table_view.model());

  EXPECT_EQ(provider.GetSelectedItem(), nullptr);
  EXPECT_EQ(provider.GetSelectedItems(), std::vector<const mvvm::SessionItem *>({}));
  EXPECT_EQ(provider.GetSelectedPointIndex(), -1);
}

TEST_F(WaveformTableComponentProviderTest, SimpleStruct)
{
  mvvm::ApplicationModel model;

  // an item representing time series
  auto data_item = model.InsertItem<mvvm::LineSeriesDataItem>();
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});

  QTableView table_view;

  WaveformTableComponentProvider provider(&model, &table_view);
  provider.SetItem(data_item);

  // validating how view sees our data via proxymodel
  auto proxy_model = provider.GetProxyModel();

  EXPECT_EQ(proxy_model->rowCount(), 2);
  EXPECT_EQ(proxy_model->columnCount(), 3);

  // first row
  auto x_col0 = proxy_model->index(0, 0);
  auto x_col1 = proxy_model->index(0, 1);
  auto x_col2 = proxy_model->index(0, 2);
  EXPECT_EQ(proxy_model->data(x_col0, Qt::DisplayRole).toDouble(), 1.0);
  EXPECT_EQ(proxy_model->data(x_col1, Qt::DisplayRole).toDouble(), 2.0);
  EXPECT_EQ(proxy_model->data(x_col2, Qt::DisplayRole).toDouble(), 3.0);

  // second row
  auto y_col0 = proxy_model->index(1, 0);
  auto y_col1 = proxy_model->index(1, 1);
  auto y_col2 = proxy_model->index(1, 2);
  EXPECT_EQ(proxy_model->data(y_col0, Qt::DisplayRole).toDouble(), 10.0);
  EXPECT_EQ(proxy_model->data(y_col1, Qt::DisplayRole).toDouble(), 20.0);
  EXPECT_EQ(proxy_model->data(y_col2, Qt::DisplayRole).toDouble(), 30.0);

  // checking if we can get to original item via visible indexes
  auto points = data_item->GetPoints();
  auto x_item = points.at(1)->GetItem(mvvm::constants::kX);
  auto y_item = points.at(1)->GetItem(mvvm::constants::kY);

  EXPECT_EQ(provider.GetItemFromViewIndex(x_col1), x_item);
  EXPECT_EQ(provider.GetItemFromViewIndex(y_col1), y_item);

  // checking if we can retrieve item's view indexes
  EXPECT_EQ(provider.GetViewIndices(x_item), QList<QModelIndex>({x_col1}));
}

TEST_F(WaveformTableComponentProviderTest, Selection)
{
  mvvm::ApplicationModel model;

  // an item representing time series
  auto data_item = model.InsertItem<mvvm::LineSeriesDataItem>();
  data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});

  QTableView table_view;

  WaveformTableComponentProvider provider(&model, &table_view);
  provider.SetItem(data_item);

  // validating how view sees our data via proxymodel
  auto proxy_model = provider.GetProxyModel();

  EXPECT_EQ(proxy_model->rowCount(), 2);
  EXPECT_EQ(proxy_model->columnCount(), 3);

  // first row
  auto x_col0 = proxy_model->index(0, 0);
  auto x_col1 = proxy_model->index(0, 1);
  auto x_col2 = proxy_model->index(0, 2);
  EXPECT_EQ(proxy_model->data(x_col0, Qt::DisplayRole).toDouble(), 1.0);
  EXPECT_EQ(proxy_model->data(x_col1, Qt::DisplayRole).toDouble(), 2.0);
  EXPECT_EQ(proxy_model->data(x_col2, Qt::DisplayRole).toDouble(), 3.0);

  // second row
  auto y_col0 = proxy_model->index(1, 0);
  auto y_col1 = proxy_model->index(1, 1);
  auto y_col2 = proxy_model->index(1, 2);
  EXPECT_EQ(proxy_model->data(y_col0, Qt::DisplayRole).toDouble(), 10.0);
  EXPECT_EQ(proxy_model->data(y_col1, Qt::DisplayRole).toDouble(), 20.0);
  EXPECT_EQ(proxy_model->data(y_col2, Qt::DisplayRole).toDouble(), 30.0);

  auto points = data_item->GetPoints();
  auto x_item = points.at(1)->GetItem(mvvm::constants::kX);
  auto y_item = points.at(1)->GetItem(mvvm::constants::kY);

  // selecting middle x-cell
  provider.SetSelectedItem(x_item);

  // should lead to the selection of the whole volumn
  EXPECT_EQ(provider.GetSelectedItems(), std::vector<const mvvm::SessionItem *>({x_item, y_item}));

  // should report correct index of selected point
  EXPECT_EQ(provider.GetSelectedPointIndex(), 1);
}
