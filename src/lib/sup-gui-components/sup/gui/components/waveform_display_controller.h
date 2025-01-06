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

#ifndef SUP_GUI_COMPONENTS_WAVEFORM_DISPLAY_CONTROLLER_H_
#define SUP_GUI_COMPONENTS_WAVEFORM_DISPLAY_CONTROLLER_H_

#include <sup/gui/components/component_types.h>

#include <mvvm/signals/event_types.h>

#include <memory>

namespace mvvm
{
class ChartViewportItem;
class LineSeriesItem;
class ModelListener;
}  // namespace mvvm

namespace sup::gui
{

/**
 * @brief The WaveformDisplayController class is responsible for showing or hiding multiple
 * waveforms on a single canvas depending on the WaveformDisplayMode setting.
 *
 * For the moment simple logic is implemented.
 *
 * Mode kDisplayAll:
 * Controller will set the "displayed" flag for all available waveforms to "true", and will ignore
 * all other activity. Users can still manipulate the "displayed" flag of the waveform manually via
 * the property editor.
 *
 * Mode kDisplaySelected
 * Controller will set "displayed" flag for waveform which is currently selected by the user.
 */
class WaveformDisplayController
{
public:
  explicit WaveformDisplayController(mvvm::ChartViewportItem* chart_viewport);
  ~WaveformDisplayController();

  void SetDisplayMode(WaveformDisplayMode display_mode);

  void SetSelected(mvvm::LineSeriesItem* selected_waveform);

private:
  /**
   * @brief Updates display status
   */
  void UpdateDisplayStatus();

  /**
   * @brief Upadte selection status of waveforms.
   */
  void OnModelEvent(const mvvm::ItemInsertedEvent& event);

  /**
   * @brief Upadte selection status of waveforms.
   */
  void OnModelEvent(const mvvm::ItemRemovedEvent& event);

  WaveformDisplayMode m_current_display_mode{WaveformDisplayMode::kDisplayAll};
  mvvm::ChartViewportItem* m_chart_viewport{nullptr};
  mvvm::LineSeriesItem* m_selected_waveform{nullptr};
  std::unique_ptr<mvvm::ModelListener> m_listener;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_WAVEFORM_DISPLAY_CONTROLLER_H_
