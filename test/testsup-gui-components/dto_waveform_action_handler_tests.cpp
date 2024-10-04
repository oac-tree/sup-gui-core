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

#include "sup/gui/components/dto_waveform_action_handler.h"

#include <sup/gui/core/exceptions.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

using namespace sup::gui;

/**
 * @brief Tests for DtoWaveformActionHandler class.
 */
class DtoWaveformActionHandlerTest : public ::testing::Test
{
public:
  DtoWaveformActionHandlerTest()
  {
    m_waveform_container = m_model.InsertItem<mvvm::ContainerItem>();
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
    result.selected_waveform = [selected_waveform]() { return selected_waveform; };

    return result;
  }

  std::unique_ptr<DtoWaveformActionHandler> CreateActionHandler(
      mvvm::LineSeriesItem* selected_waveform)
  {
    return std::make_unique<DtoWaveformActionHandler>(CreateContext(selected_waveform));
  }

  mvvm::ApplicationModel m_model;
  mvvm::SessionItem* m_waveform_container{nullptr};
};

TEST_F(DtoWaveformActionHandlerTest, AttemptToCreateHandlerWithoutCallbacks)
{
  EXPECT_THROW(DtoWaveformActionHandler({}), RuntimeException);

  auto waveform_container_callback = [this]() { return m_waveform_container; };
  auto select_waveform_callback = []() { return nullptr; };

  EXPECT_THROW(DtoWaveformActionHandler({}), RuntimeException);
  EXPECT_THROW(DtoWaveformActionHandler({{}, select_waveform_callback}), RuntimeException);
  EXPECT_THROW(DtoWaveformActionHandler({waveform_container_callback, {}}), RuntimeException);
}
