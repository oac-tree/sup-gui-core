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

#include "message_handler_factory.h"

#include "widget_overlay_message_handler.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/core/message_handler_decorator.h>
#include <sup/gui/core/standard_message_handlers.h>

namespace sup::gui
{

std::unique_ptr<IMessageHandler> CreateNullMessageHandler()
{
  return std::make_unique<NullMessageHandler>();
}

std::unique_ptr<IMessageHandler> CreateStdMessageHandler()
{
  return std::make_unique<StdMessageHandler>();
}

std::unique_ptr<IMessageHandler> CreateWidgetOverlayMessageHandler(QWidget *view)
{
  return std::make_unique<WidgetOverlayMessageHandler>(view);
}

std::unique_ptr<IMessageHandler> CreateMessageHandlerDecorator(
    IMessageHandler *component)
{
  return MessageHandlerDecorator::Create(component);
}

std::unique_ptr<IMessageHandler> CreateThrowingMessageHandler()
{
  return std::make_unique<ThrowingMessageHandler<RuntimeException>>();
}

}  // namespace sup::gui
