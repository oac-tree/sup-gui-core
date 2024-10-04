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

#include "dto_waveform_action_handler.h"

#include <sup/gui/core/exceptions.h>

namespace sup::gui
{

DtoWaveformActionHandler::DtoWaveformActionHandler(DtoWaveformEditorContext context,
                                                   QObject *parent)
    : QObject(parent), m_context(std::move(context))
{
  if (!m_context.waveform_container)
  {
    throw RuntimeException("Callback to retrieve waveform container is not defined");
  }

  if (!m_context.selected_waveform)
  {
    throw RuntimeException("Callback to retrieve selected waveform is not defined");
  }
}

void DtoWaveformActionHandler::AddWaveform() {}

void DtoWaveformActionHandler::RemoveWaveform() {}

}  // namespace sup::gui
