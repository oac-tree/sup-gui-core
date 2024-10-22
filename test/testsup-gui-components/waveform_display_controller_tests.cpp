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

#include "sup/gui/components/waveform_display_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/line_series_item.h>

#include <gtest/gtest.h>

#include <QMimeData>

using namespace sup::gui;

class WaveformDisplayControllerTest : public ::testing::Test
{
};

TEST_F(WaveformDisplayControllerTest, DisplayAllMode)
{
  mvvm::ApplicationModel model;
  auto viewport = model.InsertItem<mvvm::ChartViewportItem>();

  auto waveform0 = model.InsertItem<mvvm::LineSeriesItem>(viewport, mvvm::TagIndex::Append());
  auto waveform1 = model.InsertItem<mvvm::LineSeriesItem>(viewport, mvvm::TagIndex::Append());

  WaveformDisplayController controller(viewport);

  // initially everything is displayed
  EXPECT_TRUE(waveform0->IsDisplayed());
  EXPECT_TRUE(waveform1->IsDisplayed());

  // selection doesn't affect display status
  controller.SetSelected(waveform0);
  EXPECT_TRUE(waveform0->IsDisplayed());
  EXPECT_TRUE(waveform1->IsDisplayed());
  controller.SetSelected(waveform1);
  EXPECT_TRUE(waveform0->IsDisplayed());
  EXPECT_TRUE(waveform1->IsDisplayed());

  // user can still change display status via property editor
  waveform0->SetDisplayed(false);
  waveform1->SetDisplayed(false);
  EXPECT_FALSE(waveform0->IsDisplayed());
  EXPECT_FALSE(waveform1->IsDisplayed());
}

//! Waveforms were initially hidden. Controller in DisplayAll mode should show them first
TEST_F(WaveformDisplayControllerTest, DisplayAllModeWhenInitiallyInvisible)
{
  mvvm::ApplicationModel model;
  auto viewport = model.InsertItem<mvvm::ChartViewportItem>();

  auto waveform0 = model.InsertItem<mvvm::LineSeriesItem>(viewport, mvvm::TagIndex::Append());
  auto waveform1 = model.InsertItem<mvvm::LineSeriesItem>(viewport, mvvm::TagIndex::Append());
  waveform0->SetDisplayed(false);
  waveform1->SetDisplayed(false);

  WaveformDisplayController controller(viewport);

  // initially everything is displayed
  EXPECT_TRUE(waveform0->IsDisplayed());
  EXPECT_TRUE(waveform1->IsDisplayed());

  // selection doesn't affect display status
  controller.SetSelected(waveform0);
  EXPECT_TRUE(waveform0->IsDisplayed());
  EXPECT_TRUE(waveform1->IsDisplayed());
  controller.SetSelected(waveform1);
  EXPECT_TRUE(waveform0->IsDisplayed());
  EXPECT_TRUE(waveform1->IsDisplayed());

  // user can still change display status via property editor
  waveform0->SetDisplayed(false);
  waveform1->SetDisplayed(false);
  EXPECT_FALSE(waveform0->IsDisplayed());
  EXPECT_FALSE(waveform1->IsDisplayed());
}

TEST_F(WaveformDisplayControllerTest, DisplaySelectedMode)
{
  mvvm::ApplicationModel model;
  auto viewport = model.InsertItem<mvvm::ChartViewportItem>();

  auto waveform0 = model.InsertItem<mvvm::LineSeriesItem>(viewport, mvvm::TagIndex::Append());
  auto waveform1 = model.InsertItem<mvvm::LineSeriesItem>(viewport, mvvm::TagIndex::Append());

  WaveformDisplayController controller(viewport);
  controller.SetDisplayMode(WaveformDisplayMode::kDisplaySelected);

  // everything will be hidden unless we mark one of waveforms as selected
  EXPECT_FALSE(waveform0->IsDisplayed());
  EXPECT_FALSE(waveform1->IsDisplayed());

  controller.SetSelected(waveform0);
  EXPECT_TRUE(waveform0->IsDisplayed());
  EXPECT_FALSE(waveform1->IsDisplayed());

  controller.SetSelected(waveform1);
  EXPECT_FALSE(waveform0->IsDisplayed());
  EXPECT_TRUE(waveform1->IsDisplayed());

  // user can still change display status via property editor
  waveform0->SetDisplayed(false);
  waveform1->SetDisplayed(false);
  EXPECT_FALSE(waveform0->IsDisplayed());
  EXPECT_FALSE(waveform1->IsDisplayed());

  // but selection of item will reset his choice
  controller.SetSelected(waveform0);
  EXPECT_TRUE(waveform0->IsDisplayed());
  EXPECT_FALSE(waveform1->IsDisplayed());
}
