/******************************************************************************
 *
 * Project       : PSPS editor
 *
 * Description   : Graphical user interface for pulse schedule preparation system
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "sup/gui/plotting/waveform_editor_action_handler.h"

#include <sup/gui/plotting/waveform_editor_context.h>
#include <sup/gui/plotting/waveform_helper.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/point_item.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(mvvm::SessionItem*)
#endif

namespace sup::gui::test
{

class WaveformEditorActionHandlerTest : public ::testing::Test
{
public:
  WaveformEditorActionHandlerTest()
  {
    m_data_item = m_model.InsertItem<mvvm::LineSeriesDataItem>();
    m_viewport_item = m_model.InsertItem<mvvm::ChartViewportItem>();
    m_line_series_item = m_model.InsertItem<mvvm::LineSeriesItem>(m_viewport_item);
    m_line_series_item->SetDataItem(m_data_item);
  }

  /**
   * @brief Helper test function to create context necessary for WaveformEditorActionHandler to
   * function.
   *
   * @param current_line_series The item which will be reported as current served line series.
   * @param selected_point The item which will be reported as "selected" item.
   */
  WaveformEditorContext CreateContext(mvvm::LineSeriesItem* current_line_series,
                                      mvvm::PointItem* selected_point)
  {
    auto get_current_line_series = [current_line_series]() { return current_line_series; };

    // callback returns given item, pretending it is user's selection of data point
    auto get_selected_point_callback = [selected_point]() { return selected_point; };

    return {get_current_line_series, get_selected_point_callback};
  }

  /**
   * @brief Helper test function to create WaveformEditorActionHandler.
   *
   * @param current_line_series The item which will be reported as current served line series.
   * @param selected_point The item which will be reported as "selected" item.
   */
  std::unique_ptr<WaveformEditorActionHandler> CreateActionHandler(
      mvvm::LineSeriesItem* current_line_series, mvvm::PointItem* selected_point)
  {
    return std::make_unique<WaveformEditorActionHandler>(
        CreateContext(current_line_series, selected_point));
  }

  /**
   * @brief Helper function that retrieves SessionItem from QSignalSpy arguments.
   */
  mvvm::SessionItem* GetSendItem(QSignalSpy& signal_spy)
  {
    if (signal_spy.count() == 1)
    {
      auto arguments = signal_spy.takeFirst();
      if (arguments.size() == 1)
      {
        return arguments.at(0).value<mvvm::SessionItem*>();
      }
    }
    return nullptr;
  }

  mvvm::ApplicationModel m_model;
  mvvm::LineSeriesDataItem* m_data_item{nullptr};
  mvvm::ChartViewportItem* m_viewport_item{nullptr};
  mvvm::LineSeriesItem* m_line_series_item{nullptr};
};

//! Appending points to empty line series.
TEST_F(WaveformEditorActionHandlerTest, AddAfterEmptyLineSeries)
{
  // Line series set, no point selected
  auto action_handler = CreateActionHandler(m_line_series_item, nullptr);

  QSignalSpy spy_selection_request(action_handler.get(),
                                   &WaveformEditorActionHandler::SelectItemRequest);

  action_handler->OnAddColumnAfterRequest();

  // should get a point (0.0)
  auto points = m_data_item->GetAllItems();
  ASSERT_EQ(m_data_item->GetPointCount(), 1);
  auto [x, y] = m_data_item->GetPointCoordinates(0);
  EXPECT_EQ(x, 0.0);
  EXPECT_EQ(y, 0.0);

  EXPECT_EQ(GetSendItem(spy_selection_request), points.at(0));
}

//! Appending points to non-empty line series. For the moment action handler has following behavior:
//! if point is selected, new point will have same "y" and slightly shifted x.
TEST_F(WaveformEditorActionHandlerTest, AddAfterNonEmptyLineSeries)
{
  m_data_item->InsertPoint(0, {1.0, 10.0});

  // Line series set, no point selected
  auto action_handler = CreateActionHandler(m_line_series_item, m_data_item->GetPoint(0));

  action_handler->OnAddColumnAfterRequest();

  // should get a point (1.1, 10.0)
  auto points = m_data_item->GetPoints();
  ASSERT_EQ(points.size(), 2);
  EXPECT_DOUBLE_EQ(points.at(1)->GetX(), 1.0 + kDefaultDx);
  EXPECT_EQ(points.at(1)->GetY(), 10.0);
}

//! Appending points to non-empty line series. The difference with previous test is that nothing is
//! selected. Nevertheless, the algorithm should add point with coordinates after the last existing
//! point.
TEST_F(WaveformEditorActionHandlerTest, AddAfterNonEmptyLineSeriesWhenNothingIsSelected)
{
  m_data_item->InsertPoint(0, {1.0, 10.0});

  // Line series set, no point selected
  auto action_handler = CreateActionHandler(m_line_series_item, nullptr);

  action_handler->OnAddColumnAfterRequest();

  // should get a point (1.1, 10.0)
  auto points = m_data_item->GetPoints();
  ASSERT_EQ(points.size(), 2);
  EXPECT_DOUBLE_EQ(points.at(1)->GetX(), 1.0 + kDefaultDx);
  EXPECT_EQ(points.at(1)->GetY(), 10.0);
}

TEST_F(WaveformEditorActionHandlerTest, RemovePoint)
{
  // Line series set, no point selected
  auto action_handler = CreateActionHandler(m_line_series_item, nullptr);

  m_data_item->InsertPoint(0, {0.0, 0.0});

  action_handler->OnRemoveColumnRequest();

  // still same amount of points
  ASSERT_EQ(m_data_item->GetPointCount(), 1);

  // creating handler mimicking the points is selected
  action_handler = CreateActionHandler(m_line_series_item, m_data_item->GetPoint(0));

  // second removal should succeed
  action_handler->OnRemoveColumnRequest();

  ASSERT_EQ(m_data_item->GetPointCount(), 0);
}

TEST_F(WaveformEditorActionHandlerTest, RemoveMiddlePoint)
{
  m_data_item->SetWaveform({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});

  // Line series set, middle point is selected
  auto action_handler = CreateActionHandler(m_line_series_item, m_data_item->GetPoint(1));

  QSignalSpy spy_selection_request(action_handler.get(),
                                   &WaveformEditorActionHandler::SelectItemRequest);

  // removing second point
  action_handler->OnRemoveColumnRequest();

  // new points should have only first and last point
  std::vector<std::pair<double, double>> expected({{1.0, 10.0}, {3.0, 30.0}});
  EXPECT_EQ(m_data_item->GetWaveform(), expected);

  // algorithm should suggest the last point for selection
  EXPECT_EQ(GetSendItem(spy_selection_request), m_data_item->GetPoint(1));
}

//! Prepending points to empty line series.

TEST_F(WaveformEditorActionHandlerTest, AddBeforeOnEmptySeries)
{
  // Line series set, no point selected
  auto action_handler = CreateActionHandler(m_line_series_item, nullptr);

  action_handler->OnAddColumnBeforeRequest();

  // should get a point (0.0)
  ASSERT_EQ(m_data_item->GetPointCount(), 1);
  auto [x, y] = m_data_item->GetPointCoordinates(0);
  EXPECT_EQ(x, 0.0);
  EXPECT_EQ(y, 0.0);
}

//! Prepending points to non-empty line series. For the moment action handler has following
//! behavior: if point is selected, new point will have same "y" and x slighly shifted back.
TEST_F(WaveformEditorActionHandlerTest, AddBeforeNonEmptyLineSeries)
{
  m_data_item->InsertPoint(0, {1.0, 10.0});

  // Line series set, point selected
  auto action_handler = CreateActionHandler(m_line_series_item, m_data_item->GetPoint(0));

  action_handler->OnAddColumnBeforeRequest();

  // should get a point (0.9, 10.0)
  ASSERT_EQ(m_data_item->GetPointCount(), 2);
  auto [x, y] = m_data_item->GetPointCoordinates(0);
  EXPECT_DOUBLE_EQ(x, 1.0 - kDefaultDx);
  EXPECT_EQ(y, 10.0);
}

//! Prepending points to non-empty line series. Nothing is selected.
//! New point should appear at the beginning, slighly shifted back.
TEST_F(WaveformEditorActionHandlerTest, AddBeforeNonEmptyLineSeriesWHenNothinIsSelected)
{
  m_data_item->InsertPoint(0, {1.0, 10.0});

  // Line series set, nothing is selected
  auto action_handler = CreateActionHandler(m_line_series_item, nullptr);

  action_handler->OnAddColumnBeforeRequest();

  // should get a point (0.9, 10.0)
  ASSERT_EQ(m_data_item->GetPointCount(), 2);
  auto [x, y] = m_data_item->GetPointCoordinates(0);
  EXPECT_DOUBLE_EQ(x, 1.0 - kDefaultDx);
  EXPECT_EQ(y, 10.0);
}

}  // namespace sup::gui::test
