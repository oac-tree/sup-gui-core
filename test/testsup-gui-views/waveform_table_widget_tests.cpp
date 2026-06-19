/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "sup/gui/views/waveformeditor/waveform_table_widget.h"

#include <sup/gui/views/waveformeditor/waveform_table_component_provider.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/item_selection.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/point_item.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QItemSelectionModel>
#include <QSignalSpy>
#include <QTableView>
#include <QTransposeProxyModel>

namespace sup::gui::test
{

/**
 * @brief Tests for WaveformTableWidget class.
 */
class WaveformTableWidgetTest : public ::testing::Test
{
public:
  WaveformTableWidgetTest()
  {
    m_data_item = m_model.InsertItem<mvvm::LineSeriesDataItem>();
    m_viewport_item = m_model.InsertItem<mvvm::ChartViewportItem>();
    m_line_series_item = m_model.InsertItem<mvvm::LineSeriesItem>(m_viewport_item);
    m_line_series_item->SetDataItem(m_data_item);
  }
  mvvm::ApplicationModel m_model;
  mvvm::LineSeriesDataItem* m_data_item{nullptr};
  mvvm::ChartViewportItem* m_viewport_item{nullptr};
  mvvm::LineSeriesItem* m_line_series_item{nullptr};
};

TEST_F(WaveformTableWidgetTest, InitialState)
{
  WaveformTableWidget widget(nullptr);

  EXPECT_EQ(widget.GetSelectedPoint(), nullptr);
  EXPECT_NE(widget.GetComponentProvider(), nullptr);

  EXPECT_NO_FATAL_FAILURE(widget.SetSelectedPoint(nullptr));
}

TEST_F(WaveformTableWidgetTest, SetLineSeriesWithSinglePoint)
{
  WaveformTableWidget widget(&m_model);
  EXPECT_EQ(widget.GetSelectedPoint(), nullptr);

  m_data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});
  auto points = m_data_item->GetPoints();
  EXPECT_EQ(m_data_item->GetPoints().size(), 3);

  auto x_item1 = points.at(1)->GetItem(mvvm::constants::kX);
  auto y_item1 = points.at(1)->GetItem(mvvm::constants::kY);
  auto x_item2 = points.at(2)->GetItem(mvvm::constants::kX);
  auto y_item2 = points.at(2)->GetItem(mvvm::constants::kY);

  // setting LineSeries, no point selection yet
  widget.SetLineSeriesItem(m_line_series_item);
  EXPECT_EQ(widget.GetLineSeriesItem(), m_line_series_item);
  EXPECT_EQ(widget.GetSelectedPoint(), nullptr);

  auto provider = widget.GetComponentProvider();
  QSignalSpy spy_selected(provider, &WaveformTableComponentProvider::SelectionChanged);

  // selecting middle point
  auto selected_point = m_data_item->GetPoints().at(1);
  widget.SetSelectedPoint(selected_point);
  EXPECT_EQ(widget.GetSelectedPoint(), selected_point);

  EXPECT_EQ(provider->GetSelection().GetItems(),
            std::vector<const mvvm::SessionItem*>({x_item1, y_item1}));
  EXPECT_EQ(mvvm::test::TakeValue<mvvm::ItemSelection>(spy_selected),
            mvvm::ItemSelection({x_item1, y_item1}));

  // selecting last point
  selected_point = m_data_item->GetPoints().at(2);
  widget.SetSelectedPoint(selected_point);
  EXPECT_EQ(widget.GetSelectedPoint(), selected_point);

  EXPECT_EQ(provider->GetSelection().GetItems(),
            std::vector<const mvvm::SessionItem*>({x_item2, y_item2}));
  EXPECT_EQ(mvvm::test::TakeValue<mvvm::ItemSelection>(spy_selected),
            mvvm::ItemSelection({x_item2, y_item2}));

  // validating selection via proxy model
  auto proxy_model = provider->GetLastProxyModel();
  auto x_col2 = proxy_model->index(0, 2);
  auto y_col2 = proxy_model->index(1, 2);

  auto table_view = provider->GetView();
  auto table_selection_model = table_view->selectionModel();
  auto selected_indexes = table_selection_model->selectedIndexes();
  EXPECT_EQ(selected_indexes.size(), 2);
  EXPECT_EQ(selected_indexes.at(0), x_col2);
  EXPECT_EQ(selected_indexes.at(1), y_col2);
}

}  // namespace sup::gui::test
