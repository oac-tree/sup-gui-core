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

#ifndef SUP_GUI_WIDGETS_WIDGET_OVERLAY_MESSAGE_HANDLER_H_
#define SUP_GUI_WIDGETS_WIDGET_OVERLAY_MESSAGE_HANDLER_H_

#include <sup/gui/core/i_message_handler.h>

#include <chrono>
#include <memory>
#include <optional>

class QWidget;

namespace sup::gui
{

class WidgetOverlayMessagePublisher;

/**
 * @brief The WidgetOverlayMessageHandler class displays messages in widget corner.
 */
class WidgetOverlayMessageHandler : public IMessageHandler
{
public:
  explicit WidgetOverlayMessageHandler(
      QWidget* view, const std::optional<std::chrono::milliseconds>& display_duration);
  ~WidgetOverlayMessageHandler() override;

  WidgetOverlayMessageHandler(const WidgetOverlayMessageHandler&) = delete;
  WidgetOverlayMessageHandler& operator=(const WidgetOverlayMessageHandler&) = delete;
  WidgetOverlayMessageHandler(WidgetOverlayMessageHandler&&) = delete;
  WidgetOverlayMessageHandler& operator=(WidgetOverlayMessageHandler&&) = delete;

  void SendMessage(const MessageEvent& message) override;

  void ClearMessages() override;

private:
  std::unique_ptr<WidgetOverlayMessagePublisher> m_message_publisher;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_WIDGET_OVERLAY_MESSAGE_HANDLER_H_
