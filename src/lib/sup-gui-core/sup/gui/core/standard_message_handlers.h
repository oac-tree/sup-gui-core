/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_CORE_STANDARD_MESSAGE_HANDLERS_H_
#define SUP_GUI_CORE_STANDARD_MESSAGE_HANDLERS_H_

//! @file
//! Collection of simple message handlers.

#include <sup/gui/core/message_handler_interface.h>

namespace sup::gui
{

/**
 * @brief The StdMessageHandler class represents a simple message handler that prints messages to
 * standard output.
 */
class StdMessageHandler : public MessageHandlerInterface
{
public:
  void SendMessage(const std::string& text) override;
};

/**
 * @brief The StdMessageHandler class represents a message handler that does nothing.
 */
class NullMessageHandler : public MessageHandlerInterface
{
public:
  void SendMessage(const std::string& text) override;
};

/**
 * @brief The StdMessageHandler class represents a message handler that throws an exception of a
 * given type.
 */
template <typename T>
class ThrowingMessageHandler : public MessageHandlerInterface
{
public:
  void SendMessage(const std::string& text) override { throw T(text); }
};

}  // namespace sup::gui

#endif  // SUP_GUI_CORE_STANDARD_MESSAGE_HANDLERS_H_
