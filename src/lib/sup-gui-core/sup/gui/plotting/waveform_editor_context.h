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

#ifndef SUP_GUI_PLOTTING_WAVEFORM_EDITOR_CONTEXT_H_
#define SUP_GUI_PLOTTING_WAVEFORM_EDITOR_CONTEXT_H_

#include <functional>

namespace sup::gui
{

class AnyValueItem;
class LineSeriesItem;

/**
 * @brief The WaveformEditorContext struct carries the data for WaveformEditorActionHandler.
 */
struct WaveformEditorContext
{
  //!< Callback to retrieve current line series item.
  std::function<LineSeriesItem*()> get_selected_waveform_callback;

  //!< Callback to retrieve currently selected AnyValueItem representing a point.
  std::function<AnyValueItem*()> get_selected_point_callback;
};

}  // namespace sup::gui

#endif  // SUP_GUI_PLOTTING_WAVEFORM_EDITOR_CONTEXT_H_
