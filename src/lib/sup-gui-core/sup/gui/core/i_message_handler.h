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

#ifndef SUP_GUI_CORE_I_MESSAGE_HANDLER_H_
#define SUP_GUI_CORE_I_MESSAGE_HANDLER_H_

#include <sup/gui/core/message_event.h>

namespace sup::gui
{

struct MessageEvent;

/**
 * @brief The IMessageHandler is an interface to send the message to the user.
 */
class IMessageHandler
{
public:
  IMessageHandler() = default;
  virtual ~IMessageHandler() = default;

  IMessageHandler(const IMessageHandler&) = delete;
  IMessageHandler& operator=(const IMessageHandler&) = delete;
  IMessageHandler(IMessageHandler&&) = delete;
  IMessageHandler& operator=(IMessageHandler&&) = delete;

  virtual void SendMessage(const MessageEvent& message) = 0;
};

}  // namespace sup::gui

#endif  // SUP_GUI_CORE_I_MESSAGE_HANDLER_H_
