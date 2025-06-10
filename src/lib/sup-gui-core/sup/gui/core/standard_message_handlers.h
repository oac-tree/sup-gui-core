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

#ifndef SUP_GUI_CORE_STANDARD_MESSAGE_HANDLERS_H_
#define SUP_GUI_CORE_STANDARD_MESSAGE_HANDLERS_H_

//! @file
//! Collection of simple message handlers.

#include <sup/gui/core/i_message_handler.h>

namespace sup::gui
{

/**
 * @brief The StdMessageHandler class represents a simple message handler that prints messages to
 * standard output.
 */
class StdMessageHandler : public IMessageHandler
{
public:
  void SendMessage(const MessageEvent& message) override;
  void ClearMessages() override;
};

/**
 * @brief The StdMessageHandler class represents a message handler that does nothing.
 */
class NullMessageHandler : public IMessageHandler
{
public:
  void SendMessage(const MessageEvent& message) override;
  void ClearMessages() override;
};

/**
 * @brief The StdMessageHandler class represents a message handler that throws an exception of a
 * given type.
 */
template <typename T>
class ThrowingMessageHandler : public IMessageHandler
{
public:
  void SendMessage(const MessageEvent& message) override { throw T(message.detailed); }
  void ClearMessages() override {}
};

}  // namespace sup::gui

#endif  // SUP_GUI_CORE_STANDARD_MESSAGE_HANDLERS_H_
