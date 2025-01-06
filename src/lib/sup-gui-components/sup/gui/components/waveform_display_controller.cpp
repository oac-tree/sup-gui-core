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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "waveform_display_controller.h"

#include <mvvm/signals/model_listener.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/line_series_item.h>

namespace sup::gui
{

WaveformDisplayController::WaveformDisplayController(mvvm::ChartViewportItem* chart_viewport)
    : m_chart_viewport(chart_viewport)
    , m_listener(std::make_unique<mvvm::ModelListener>(chart_viewport->GetModel()))
{
  m_listener->Connect<mvvm::ItemInsertedEvent>(this, &WaveformDisplayController::OnModelEvent);
  m_listener->Connect<mvvm::ItemRemovedEvent>(this, &WaveformDisplayController::OnModelEvent);
  UpdateDisplayStatus();
}

WaveformDisplayController::~WaveformDisplayController() = default;

void WaveformDisplayController::SetDisplayMode(WaveformDisplayMode display_mode)
{
  m_current_display_mode = display_mode;
  UpdateDisplayStatus();
}

void WaveformDisplayController::SetSelected(mvvm::LineSeriesItem* selected_waveform)
{
  m_selected_waveform = selected_waveform;
  if (m_current_display_mode == WaveformDisplayMode::kDisplayAll)
  {
    return;
  }

  UpdateDisplayStatus();
}

void WaveformDisplayController::UpdateDisplayStatus()
{
  for (auto waveform : m_chart_viewport->GetLineSeries())
  {
    if (m_current_display_mode == WaveformDisplayMode::kDisplayAll)
    {
      waveform->SetDisplayed(true);
    }
    if (m_current_display_mode == WaveformDisplayMode::kDisplaySelected)
    {
      waveform->SetDisplayed(waveform == m_selected_waveform);
    }
  }
}

void WaveformDisplayController::OnModelEvent(const mvvm::ItemInsertedEvent& event)
{
  if (m_current_display_mode == WaveformDisplayMode::kDisplayAll)
  {
    return;
  }

  UpdateDisplayStatus();
}

void WaveformDisplayController::OnModelEvent(const mvvm::ItemRemovedEvent& event)
{
  if (m_current_display_mode == WaveformDisplayMode::kDisplayAll)
  {
    return;
  }

  UpdateDisplayStatus();
}

}  // namespace sup::gui
