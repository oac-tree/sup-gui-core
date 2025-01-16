/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "overlay_message.h"

#include "overlay_widget_position_strategy.h"
#include "warning_sign_widget.h"

namespace sup::gui
{

OverlayMessage::~OverlayMessage() = default;

OverlayMessage::OverlayMessage(const MessageEvent &message, QWidget *area)
{
  m_message_widget = std::make_unique<WarningSignWidget>(message, area);
  m_position_strategy =
      std::make_unique<OverlayWidgetPositionStrategy>(m_message_widget.get(), area);
}

bool OverlayMessage::CanBeDeleted()
{
  if (auto widget = dynamic_cast<WarningSignWidget *>(m_message_widget.get()); widget)
  {
    return !widget->IsBusy();
  }
  return true;
}

}  // namespace sup::gui
