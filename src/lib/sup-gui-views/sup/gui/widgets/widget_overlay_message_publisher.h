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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_WIDGETS_WIDGET_OVERLAY_MESSAGE_PUBLISHER_H_
#define SUP_GUI_WIDGETS_WIDGET_OVERLAY_MESSAGE_PUBLISHER_H_

#include <sup/gui/core/message_event.h>

#include <QObject>
#include <chrono>
#include <memory>

class QWidget;
class QTimer;

namespace sup::gui
{

class OverlayMessage;

/**
 * @brief The WidgetOverlayMessagePublisher class controls the appearance of the overlay messages on
 * top of arbitrary widgets.
 *
 * The current simplified implementation allows a single message at the corner of the target widget.
 */
class WidgetOverlayMessagePublisher : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief C-tor for widget publisher, which shows the message for the given time.
   */
  WidgetOverlayMessagePublisher(QWidget* target_widget,
                                const std::optional<std::chrono::milliseconds>& display_duration);

  ~WidgetOverlayMessagePublisher() override;

  WidgetOverlayMessagePublisher(const WidgetOverlayMessagePublisher&) = delete;
  WidgetOverlayMessagePublisher& operator=(const WidgetOverlayMessagePublisher&) = delete;
  WidgetOverlayMessagePublisher(WidgetOverlayMessagePublisher&&) = delete;
  WidgetOverlayMessagePublisher& operator=(WidgetOverlayMessagePublisher&&) = delete;

  void AddMessage(const MessageEvent& message);

  void ClearMessages();

private:
  void RemoveMessageOnTimeout();
  std::unique_ptr<QTimer> CreateTimer(std::chrono::milliseconds timeout) const;

  QWidget* m_target_widget{nullptr};
  std::unique_ptr<OverlayMessage> m_message;
  std::unique_ptr<QTimer> m_timer;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_WIDGET_OVERLAY_MESSAGE_PUBLISHER_H_
