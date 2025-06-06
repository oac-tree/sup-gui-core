/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "sup/gui/components/dto_waveform_action_handler.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(mvvm::LineSeriesItem*)
#endif

namespace sup::gui::test
{

/**
 * @brief Tests for DtoWaveformActionHandler class.
 */
class DtoWaveformActionHandlerTest : public ::testing::Test
{
public:
  DtoWaveformActionHandlerTest()
  {
    m_waveform_container = m_model.InsertItem<mvvm::ChartViewportItem>();
    m_data_container = m_model.InsertItem<mvvm::ContainerItem>();
  }

  /**
   * @brief Creates context representing status of some widget.
   *
   * @param selected_waveform Will be reported as currently selected waveform.
   */
  DtoWaveformEditorContext CreateContext(mvvm::LineSeriesItem* selected_waveform)
  {
    DtoWaveformEditorContext result;

    result.waveform_container = [this]() { return m_waveform_container; };
    result.data_container = [this]() { return m_data_container; };
    result.selected_waveform = [selected_waveform]() { return selected_waveform; };

    return result;
  }

  /**
   * @brief Creates action handler.
   *
   * @param selected_waveform Will be reported as currently selected waveform.
   */
  std::unique_ptr<DtoWaveformActionHandler> CreateActionHandler(
      mvvm::LineSeriesItem* selected_waveform)
  {
    return std::make_unique<DtoWaveformActionHandler>(CreateContext(selected_waveform));
  }

  /**
   * @brief Returns container used to store waveforms.
   */
  mvvm::ChartViewportItem* GetWaveformContainer() { return m_waveform_container; }

  /**
   * @brief Returns container used to store waveform data.
   */
  mvvm::ContainerItem* GetDataContainer() { return m_data_container; }

  mvvm::ApplicationModel m_model;
  mvvm::ChartViewportItem* m_waveform_container{nullptr};
  mvvm::ContainerItem* m_data_container{nullptr};
};

TEST_F(DtoWaveformActionHandlerTest, AttemptToCreateHandlerWithoutCallbacks)
{
  EXPECT_THROW(DtoWaveformActionHandler({}), RuntimeException);

  auto waveform_container_callback = [this]() { return m_waveform_container; };
  auto data_container_callback = [this]() { return m_data_container; };
  auto select_waveform_callback = []() { return nullptr; };

  EXPECT_THROW(DtoWaveformActionHandler({}), RuntimeException);
  EXPECT_THROW(DtoWaveformActionHandler({waveform_container_callback, {}}), RuntimeException);
  EXPECT_THROW(DtoWaveformActionHandler({{}, data_container_callback}), RuntimeException);
  EXPECT_THROW(DtoWaveformActionHandler({{}, {}, select_waveform_callback}), RuntimeException);
}

TEST_F(DtoWaveformActionHandlerTest, InitialState)
{
  auto waveform_container_callback = [this]() { return nullptr; };
  auto data_container_callback = [this]() { return nullptr; };
  auto select_waveform_callback = []() { return nullptr; };

  DtoWaveformActionHandler handler(
      {waveform_container_callback, data_container_callback, select_waveform_callback});

  EXPECT_FALSE(handler.CanAddWaveform());
  EXPECT_FALSE(handler.CanRemoveWaveform());
}

TEST_F(DtoWaveformActionHandlerTest, AddWaveformToEmptyModel)
{
  auto handler = CreateActionHandler(nullptr);

  EXPECT_TRUE(handler->CanAddWaveform());
  EXPECT_FALSE(handler->CanRemoveWaveform());
  EXPECT_EQ(GetWaveformContainer()->GetLineSeriesCount(), 0);
  EXPECT_TRUE(GetDataContainer()->IsEmpty());

  QSignalSpy spy_selection_request(handler.get(), &DtoWaveformActionHandler::SelectWaveformRequest);

  handler->AddWaveform();

  // new item appeared in the container
  EXPECT_EQ(GetWaveformContainer()->GetLineSeriesCount(), 1);
  ASSERT_FALSE(GetDataContainer()->IsEmpty());

  auto inserted_waveform =
      dynamic_cast<mvvm::LineSeriesItem*>(GetWaveformContainer()->GetLineSeries().at(0));
  auto inserted_data = GetDataContainer()->GetAllItems().at(0);
  ASSERT_NE(inserted_waveform, nullptr);

  ASSERT_EQ(inserted_waveform->GetDataItem(), inserted_data);

  // request to select just inserted waveform
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::LineSeriesItem*>(spy_selection_request),
            inserted_waveform);
}

TEST_F(DtoWaveformActionHandlerTest, InsertWaveformBetweenTwoWaveforms)
{
  auto waveform0 =
      m_model.InsertItem<mvvm::LineSeriesItem>(GetWaveformContainer(), mvvm::TagIndex::Append());
  auto waveform1 =
      m_model.InsertItem<mvvm::LineSeriesItem>(GetWaveformContainer(), mvvm::TagIndex::Append());
  auto data0 =
      m_model.InsertItem<mvvm::LineSeriesDataItem>(GetDataContainer(), mvvm::TagIndex::Append());
  auto data1 =
      m_model.InsertItem<mvvm::LineSeriesDataItem>(GetDataContainer(), mvvm::TagIndex::Append());

  waveform0->SetDataItem(data0);
  waveform1->SetDataItem(data1);

  // first waveform is selected
  auto handler = CreateActionHandler(waveform0);

  EXPECT_TRUE(handler->CanAddWaveform());
  EXPECT_TRUE(handler->CanRemoveWaveform());
  EXPECT_EQ(GetWaveformContainer()->GetLineSeriesCount(), 2);
  EXPECT_EQ(GetDataContainer()->GetSize(), 2);

  QSignalSpy spy_selection_request(handler.get(), &DtoWaveformActionHandler::SelectWaveformRequest);

  handler->AddWaveform();

  // new item appeared in the container in-between of two other items
  EXPECT_EQ(GetWaveformContainer()->GetLineSeriesCount(), 3);
  EXPECT_EQ(GetDataContainer()->GetSize(), 3);

  auto inserted_waveform =
      dynamic_cast<mvvm::LineSeriesItem*>(GetWaveformContainer()->GetLineSeries().at(1));
  auto inserted_data = GetDataContainer()->GetAllItems().at(1);
  ASSERT_NE(inserted_waveform, nullptr);

  ASSERT_EQ(inserted_waveform->GetDataItem(), inserted_data);

  // request to select just inserted waveform
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::LineSeriesItem*>(spy_selection_request),
            inserted_waveform);

  EXPECT_EQ(GetWaveformContainer()->GetLineSeries(),
            std::vector<mvvm::LineSeriesItem*>({waveform0, inserted_waveform, waveform1}));

  EXPECT_EQ(GetDataContainer()->GetChildren(),
            std::vector<mvvm::SessionItem*>({data0, inserted_data, data1}));
}

TEST_F(DtoWaveformActionHandlerTest, RemoveWaveformFromTheMiddle)
{
  auto waveform0 =
      m_model.InsertItem<mvvm::LineSeriesItem>(GetWaveformContainer(), mvvm::TagIndex::Append());
  auto waveform1 =
      m_model.InsertItem<mvvm::LineSeriesItem>(GetWaveformContainer(), mvvm::TagIndex::Append());
  auto waveform2 =
      m_model.InsertItem<mvvm::LineSeriesItem>(GetWaveformContainer(), mvvm::TagIndex::Append());
  auto data0 =
      m_model.InsertItem<mvvm::LineSeriesDataItem>(GetDataContainer(), mvvm::TagIndex::Append());
  auto data1 =
      m_model.InsertItem<mvvm::LineSeriesDataItem>(GetDataContainer(), mvvm::TagIndex::Append());
  auto data2 =
      m_model.InsertItem<mvvm::LineSeriesDataItem>(GetDataContainer(), mvvm::TagIndex::Append());

  waveform0->SetDataItem(data0);
  waveform1->SetDataItem(data1);
  waveform2->SetDataItem(data2);

  // first waveform is selected
  auto handler = CreateActionHandler(waveform1);

  EXPECT_TRUE(handler->CanAddWaveform());
  EXPECT_TRUE(handler->CanRemoveWaveform());

  QSignalSpy spy_selection_request(handler.get(), &DtoWaveformActionHandler::SelectWaveformRequest);

  handler->RemoveWaveform();

  // two items (waveform and its data) have been removed from both containers
  EXPECT_EQ(GetWaveformContainer()->GetLineSeriesCount(), 2);
  EXPECT_EQ(GetDataContainer()->GetSize(), 2);

  // request to select next waveform after the one which was just removed
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::LineSeriesItem*>(spy_selection_request), waveform2);

  EXPECT_EQ(GetWaveformContainer()->GetLineSeries(),
            std::vector<mvvm::LineSeriesItem*>({waveform0, waveform2}));

  EXPECT_EQ(GetDataContainer()->GetChildren(), std::vector<mvvm::SessionItem*>({data0, data2}));
}

}  // namespace sup::gui::test
