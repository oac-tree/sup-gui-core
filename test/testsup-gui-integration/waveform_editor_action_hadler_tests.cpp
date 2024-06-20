/******************************************************************************
 *
 * Project       : PSPS editor
 *
 * Description   : Graphical user interface for pulse schedule preparation system
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

#include "sup/gui/plotting/waveform_editor_action_handler.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/plotting/chart_items.h>
#include <sup/gui/plotting/waveform_helper.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>
#include <sup/gui/plotting/waveform_editor_context.h>

#include <QSignalSpy>

using namespace sup::gui;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

class WaveformEditorActionHandlerTest : public ::testing::Test
{
public:
  WaveformEditorActionHandlerTest()
  {
    m_data_item = m_model.InsertItem<AnyValueArrayItem>();
    m_viewport_item = m_model.InsertItem<ChartViewportItem>();
    m_line_series_item = m_model.InsertItem<LineSeriesItem>(m_viewport_item);
    m_line_series_item->SetDataItem(m_data_item);
  }

  /**
   * @brief Helper test function to create context necessary for WaveformEditorActionHandler to
   * function.
   *
   * @param current_line_series The item which will be reported as current served line series.
   * @param selected_point The item which will be reported as "selected" item.
   */
  WaveformEditorContext CreateContext(LineSeriesItem* current_line_series,
                                      AnyValueItem* selected_point)
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
      LineSeriesItem* current_line_series, AnyValueItem* selected_point)
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
  AnyValueArrayItem* m_data_item{nullptr};
  ChartViewportItem* m_viewport_item{nullptr};
  LineSeriesItem* m_line_series_item{nullptr};
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
  auto points = m_data_item->GetChildren();
  ASSERT_EQ(points.size(), 1);
  auto [x, y] = GetXY(*points.at(0));
  EXPECT_EQ(x, 0.0);
  EXPECT_EQ(y, 0.0);

  EXPECT_EQ(GetSendItem(spy_selection_request), points.at(0));
}

//! Appending points to non-empty line series. For the moment action handler has following behavior:
//! if point is selected, new point will have same "y" and slightly shifted x.

TEST_F(WaveformEditorActionHandlerTest, AddAfterNonEmptyLineSeries)
{
  auto point = CreatePoint(1.0, 10.0);
  auto point_ptr = point.get();
  m_model.InsertItem(std::move(point), m_data_item, mvvm::TagIndex::Append());

  // Line series set, no point selected
  auto action_handler = CreateActionHandler(m_line_series_item, point_ptr);

  action_handler->OnAddColumnAfterRequest();

  // should get a point (1.1, 10.0)
  auto points = m_data_item->GetChildren();
  ASSERT_EQ(points.size(), 2);
  auto [x, y] = GetXY(*points.at(1));
  EXPECT_DOUBLE_EQ(x, 1.0 + kDefaultDx);
  EXPECT_DOUBLE_EQ(y, 10.0);
}

//! Appending points to non-empty line series. The difference with previous test is that nothing is
//! selected. Nevertheless, the algorithm should add point with coordinates after the last existing
//! point.

TEST_F(WaveformEditorActionHandlerTest, AddAfterNonEmptyLineSeriesWhenNothingIsSelected)
{
  auto point = CreatePoint(1.0, 10.0);
  auto point_ptr = point.get();
  m_model.InsertItem(std::move(point), m_data_item, mvvm::TagIndex::Append());

  // Line series set, no point selected
  auto action_handler = CreateActionHandler(m_line_series_item, nullptr);

  action_handler->OnAddColumnAfterRequest();

  // should get a point (1.1, 10.0)
  auto points = m_data_item->GetChildren();
  ASSERT_EQ(points.size(), 2);
  auto [x, y] = GetXY(*points.at(1));
  EXPECT_DOUBLE_EQ(x, 1.0 + kDefaultDx);
  EXPECT_DOUBLE_EQ(y, 10.0);
}

TEST_F(WaveformEditorActionHandlerTest, RemovePoint)
{
  // Line series set, no point selected
  auto action_handler = CreateActionHandler(m_line_series_item, nullptr);

  auto point = CreatePoint(0.0, 0.0);
  auto point_ptr = point.get();
  m_model.InsertItem(std::move(point), m_data_item, mvvm::TagIndex::Append());

  action_handler->OnRemoveColumnRequest();

  // still same amount of points
  auto points = m_data_item->GetChildren();
  ASSERT_EQ(points.size(), 1);

  // creating handler mimicking the points is selected
  action_handler = CreateActionHandler(m_line_series_item, point_ptr);

  // second removal should succeed
  action_handler->OnRemoveColumnRequest();

  EXPECT_TRUE(m_data_item->GetChildren().empty());
}

TEST_F(WaveformEditorActionHandlerTest, RemoveMiddlePoint)
{
  m_model.InsertItem(CreatePoint(1.0, 10.0), m_data_item, mvvm::TagIndex::Append());
  m_model.InsertItem(CreatePoint(2.0, 20.0), m_data_item, mvvm::TagIndex::Append());
  m_model.InsertItem(CreatePoint(3.0, 30.0), m_data_item, mvvm::TagIndex::Append());
  auto points = m_data_item->GetChildren();

  // Line series set, middle point is selected
  auto action_handler = CreateActionHandler(m_line_series_item, points.at(1));

  QSignalSpy spy_selection_request(action_handler.get(),
                                   &WaveformEditorActionHandler::SelectItemRequest);

  // removing second point
  action_handler->OnRemoveColumnRequest();

  // new points should have only first and last point
  EXPECT_EQ(m_data_item->GetChildren(),
            std::vector<AnyValueItem*>({points.at(0), points.at(2)}));

  // algorithm should suggest the last point for selection
  EXPECT_EQ(GetSendItem(spy_selection_request), points.at(2));
}

//! Prepending points to empty line series.

TEST_F(WaveformEditorActionHandlerTest, AddBeforeOnEmptySeries)
{
  // Line series set, no point selected
  auto action_handler = CreateActionHandler(m_line_series_item, nullptr);

  action_handler->OnAddColumnBeforeRequest();

  // should get a point (0.0)
  auto points = m_data_item->GetChildren();
  ASSERT_EQ(points.size(), 1);
  auto [x, y] = GetXY(*points.at(0));
  EXPECT_EQ(x, 0.0);
  EXPECT_EQ(y, 0.0);
}

//! Prepending points to non-empty line series. For the moment action handler has following
//! behavior: if point is selected, new point will have same "y" and x slighly shifted back.

TEST_F(WaveformEditorActionHandlerTest, AddBeforeNonEmptyLineSeries)
{
  auto point = CreatePoint(1.0, 10.0);
  auto point_ptr = point.get();
  m_model.InsertItem(std::move(point), m_data_item, mvvm::TagIndex::Append());

  // Line series set, point selected
  auto action_handler = CreateActionHandler(m_line_series_item, point_ptr);

  action_handler->OnAddColumnBeforeRequest();

  // should get a point (0.9, 10.0)
  auto points = m_data_item->GetChildren();
  ASSERT_EQ(points.size(), 2);
  auto [x, y] = GetXY(*points.at(0));
  EXPECT_DOUBLE_EQ(x, 1.0 - kDefaultDx);
  EXPECT_DOUBLE_EQ(y, 10.0);
}

//! Prepending points to non-empty line series. Nothing is selected.
//! New point should appear at the beginning, slighly shifted back.

TEST_F(WaveformEditorActionHandlerTest, AddBeforeNonEmptyLineSeriesWHenNothinIsSelected)
{
  auto point = CreatePoint(1.0, 10.0);
  auto point_ptr = point.get();
  m_model.InsertItem(std::move(point), m_data_item, mvvm::TagIndex::Append());

  // Line series set, nothing is selected
  auto action_handler = CreateActionHandler(m_line_series_item, nullptr);

  action_handler->OnAddColumnBeforeRequest();

  // should get a point (0.9, 10.0)
  auto points = m_data_item->GetChildren();
  ASSERT_EQ(points.size(), 2);
  auto [x, y] = GetXY(*points.at(0));
  EXPECT_DOUBLE_EQ(x, 1.0 - kDefaultDx);
  EXPECT_DOUBLE_EQ(y, 10.0);
}
