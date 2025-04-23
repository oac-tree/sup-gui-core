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

#ifndef SUP_GUI_CORE_MESSAGE_HANDLER_DECORATOR_H_
#define SUP_GUI_CORE_MESSAGE_HANDLER_DECORATOR_H_

#include <sup/gui/core/i_message_handler.h>

#include <memory>

namespace sup::gui
{

/**
 * @brief The MessageHandlerDecorator class is decorator for message handler.
 *
 * Used when we have to use MessageHandler with unique_ptr but still don't want to pass ownership.
 */
class MessageHandlerDecorator : public IMessageHandler
{
public:
  explicit MessageHandlerDecorator(IMessageHandler* component);

  static std::unique_ptr<IMessageHandler> Create(IMessageHandler* component);

  void SendMessage(const MessageEvent& message) override;

private:
  IMessageHandler* m_component{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_CORE_MESSAGE_HANDLER_DECORATOR_H_
