/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "widget_overlay_message_handler.h"

#include "widget_overlay_message_publisher.h"

namespace sup::gui
{

WidgetOverlayMessageHandler::WidgetOverlayMessageHandler(
    QWidget* view, const std::optional<std::chrono::milliseconds>& display_duration)
    : m_message_publisher(std::make_unique<WidgetOverlayMessagePublisher>(view, display_duration))
{
}

WidgetOverlayMessageHandler::~WidgetOverlayMessageHandler() = default;

void WidgetOverlayMessageHandler::SendMessage(const MessageEvent& message)
{
  m_message_publisher->AddMessage(message);
}

void WidgetOverlayMessageHandler::ClearMessages()
{
  m_message_publisher->ClearMessages();
}

}  // namespace sup::gui
