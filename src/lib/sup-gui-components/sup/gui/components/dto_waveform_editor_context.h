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

#ifndef SUP_GUI_COMPONENTS_DTO_WAVEFORM_EDITOR_CONTEXT_H_
#define SUP_GUI_COMPONENTS_DTO_WAVEFORM_EDITOR_CONTEXT_H_

#include <QObject>
#include <functional>

namespace mvvm
{
class LineSeriesItem;
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{

/**
 * @brief The DtoWaveformEditorContext struct holds collection of callbacks to retrieve
 */
struct DtoWaveformEditorContext
{
  //! callback to get container with waveforms
  std::function<mvvm::SessionItem*()> waveform_container;

  //! callback to get currently selected waveform
  std::function<mvvm::LineSeriesItem*()> selected_waveform;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_DTO_WAVEFORM_EDITOR_CONTEXT_H_
