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

#ifndef SUP_GUI_CORE_MESSAGE_HANDLER_DECORATOR_H_
#define SUP_GUI_CORE_MESSAGE_HANDLER_DECORATOR_H_

#include <sup/gui/core/message_handler_interface.h>

#include <memory>

namespace sup::gui
{

/**
 * @brief The MessageHandlerDecorator class is decorator for message handler.
 *
 * Used when we have to use MessageHandler with unique_ptr but still don't want to pass ownership.
 */
class MessageHandlerDecorator : public MessageHandlerInterface
{
public:
  explicit MessageHandlerDecorator(MessageHandlerInterface* component);

  static std::unique_ptr<MessageHandlerInterface> Create(MessageHandlerInterface* component);

  void SendMessage(const MessageEvent& message) override;

private:
  MessageHandlerInterface* m_component{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_CORE_MESSAGE_HANDLER_DECORATOR_H_
