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

#include "message_handler_decorator.h"

namespace sup::gui
{

MessageHandlerDecorator::MessageHandlerDecorator(MessageHandlerInterface *component)
    : m_component(component)
{
}

std::unique_ptr<MessageHandlerInterface> MessageHandlerDecorator::Create(
    MessageHandlerInterface *component)
{
  return std::make_unique<MessageHandlerDecorator>(component);
}

void MessageHandlerDecorator::SendMessage(const MessageEvent &message)
{
  m_component->SendMessage(message);
}

}  // namespace sup::gui
