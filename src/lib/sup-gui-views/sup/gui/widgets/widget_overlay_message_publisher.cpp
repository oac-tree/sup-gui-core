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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "widget_overlay_message_publisher.h"

#include "overlay_message.h"

#include <QTimer>

namespace sup::gui
{

WidgetOverlayMessagePublisher::WidgetOverlayMessagePublisher(QWidget *target_widget)
    : m_target_widget(target_widget)
{
}

WidgetOverlayMessagePublisher::WidgetOverlayMessagePublisher(QWidget *target_widget,
                                                             int32_t timeout_msec)
    : m_target_widget(target_widget), m_timer(CreateTimer(timeout_msec))
{
}

WidgetOverlayMessagePublisher::~WidgetOverlayMessagePublisher() = default;

void WidgetOverlayMessagePublisher::AddMessage(const MessageEvent &message)
{
  // in the current implementation new message replaces the old one
  m_message = std::make_unique<OverlayMessage>(message, m_target_widget);

  if (m_timer)
  {
    m_timer->start();  // starting timer to delete a message after a while
  }
}

void WidgetOverlayMessagePublisher::ClearMessages()
{
  m_message.reset();
}

void WidgetOverlayMessagePublisher::RemoveMessageOnTimeout()
{
  if (m_message && m_message->CanBeDeleted())
  {
    m_message.reset();
  }
  else
  {
    // if message can't be deleted now, repeat attempt
    m_timer->start();
  }
}

std::unique_ptr<QTimer> WidgetOverlayMessagePublisher::CreateTimer(int32_t timeout_msec)
{
  auto result = std::make_unique<QTimer>();
  result->setSingleShot(true);
  result->setInterval(timeout_msec);
  QObject::connect(result.get(), &QTimer::timeout, this,
                   &WidgetOverlayMessagePublisher::RemoveMessageOnTimeout);
  return result;
}

}  // namespace sup::gui
