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

#ifndef SEQUENCERGUI_WIDGETS_WIDGET_OVERLAY_MESSAGE_HANDLER_H_
#define SEQUENCERGUI_WIDGETS_WIDGET_OVERLAY_MESSAGE_HANDLER_H_

#include <sup/gui/core/message_handler_interface.h>

#include <memory>

class QWidget;

namespace sup::gui
{

class WidgetOverlayMessagePublisher;

/**
 * @brief The WidgetOverlayMessageHandler class displays messages in widget corner.
 */
class WidgetOverlayMessageHandler : public sup::gui::MessageHandlerInterface
{
public:
  explicit WidgetOverlayMessageHandler(QWidget* view);
  ~WidgetOverlayMessageHandler() override;

  void SendMessage(const sup::gui::MessageEvent& message) override;

private:
  std::unique_ptr<WidgetOverlayMessagePublisher> m_message_publisher;
};

}  // namespace sup::gui

#endif  // SEQUENCERGUI_WIDGETS_WIDGET_OVERLAY_MESSAGE_HANDLER_H_
