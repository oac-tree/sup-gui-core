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

#include "standard_message_handlers.h"

#include <iostream>

namespace sup::gui
{

void StdMessageHandler::SendMessage(const MessageEvent &message)
{
  std::cout << message.title << "\n";
  std::cout << message.text << "\n";
  std::cout << message.informative << "\n";
  std::cout << message.detailed << "\n";
}

void StdMessageHandler::ClearMessages()
{
  // nothing to do
}

void NullMessageHandler::SendMessage(const MessageEvent &message)
{
  (void)message;
}

void NullMessageHandler::ClearMessages()
{
   // nothing to do
}

}  // namespace sup::gui
