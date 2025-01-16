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

#ifndef SUP_GUI_WIDGETS_WIDGET_OVERLAY_MESSAGE_PUBLISHER_H_
#define SUP_GUI_WIDGETS_WIDGET_OVERLAY_MESSAGE_PUBLISHER_H_

#include <sup/gui/core/message_event.h>

#include <QObject>
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
  explicit WidgetOverlayMessagePublisher(QWidget* target_widget);
  ~WidgetOverlayMessagePublisher() override;

  void AddMessage(const MessageEvent& message);

private:
  void RemoveMessageOnTimeout();

  QWidget* m_target_widget{nullptr};
  std::unique_ptr<OverlayMessage> m_message;
  QTimer* m_timer{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_WIDGET_OVERLAY_MESSAGE_PUBLISHER_H_
