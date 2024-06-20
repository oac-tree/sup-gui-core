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

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/plotting/waveform_helper.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QTableView>
#include <QTransposeProxyModel>

using namespace sup::gui;

class WaveformTwoComponentProviderTest : public ::testing::Test
{
};

TEST_F(WaveformTwoComponentProviderTest, InitialState)
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

TEST_F(WaveformTwoComponentProviderTest, SimpleStruct)
{
  mvvm::ApplicationModel model;

  // an item representing time series
  auto array_item = sup::gui::CreatePlotData({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});
  auto array_item_ptr = array_item.get();

  model.InsertItem(std::move(array_item), model.GetRootItem(), mvvm::TagIndex::Append());

  QTableView table_view;

  WaveformTableComponentProvider provider(&model, &table_view);
  provider.SetItem(array_item_ptr);

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
  auto points = array_item_ptr->GetChildren();
  auto fields = points.at(1)->GetChildren();  // x,y of the middle points

  EXPECT_EQ(provider.GetItemFromViewIndex(x_col1), fields.at(0));
  EXPECT_EQ(provider.GetItemFromViewIndex(y_col1), fields.at(1));

  // checking if we can retrieve item's view indexes
  EXPECT_EQ(provider.GetViewIndices(fields.at(0)), QList<QModelIndex>({x_col1}));
}

TEST_F(WaveformTwoComponentProviderTest, Selection)
{
  mvvm::ApplicationModel model;

  // an item representing time series
  auto array_item = sup::gui::CreatePlotData({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});
  auto array_item_ptr = array_item.get();

  model.InsertItem(std::move(array_item), model.GetRootItem(), mvvm::TagIndex::Append());

  QTableView table_view;

  WaveformTableComponentProvider provider(&model, &table_view);
  provider.SetItem(array_item_ptr);

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

  auto points = array_item_ptr->GetChildren();
  auto fields = points.at(1)->GetChildren();  // x,y of the middle points

  // selecting middle x-cell
  provider.SetSelectedItem(fields.at(0));

  // should lead to the selection of the whole volumn
  EXPECT_EQ(provider.GetSelectedItems(),
            std::vector<const mvvm::SessionItem *>({fields.at(0), fields.at(1)}));

  // should report correct index of selected point
  EXPECT_EQ(provider.GetSelectedPointIndex(), 1);
}
