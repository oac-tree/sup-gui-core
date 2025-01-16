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

#ifndef SUP_GUI_WIDGETS_MESSAGE_HANDLER_FACTORY_H_
#define SUP_GUI_WIDGETS_MESSAGE_HANDLER_FACTORY_H_

//! @file
//! Factory functions to create various message reporters.

#include <sup/gui/core/message_handler_interface.h>

#include <memory>

class QWidget;

namespace sup::gui
{

/**
 * @brief Creates message handler that does nothing.
 */
std::unique_ptr<MessageHandlerInterface> CreateNullMessageHandler();

/**
 * @brief Creates message handler that prints messages to standard output.
 */
std::unique_ptr<MessageHandlerInterface> CreateStdMessageHandler();

/**
 * @brief Creates message handler intended to work with the graphics view.
 */
std::unique_ptr<MessageHandlerInterface> CreateWidgetOverlayMessageHandler(QWidget* view);

/**
 * @brief Creates message handler decorator.
 */
std::unique_ptr<MessageHandlerInterface> CreateMessageHandlerDecorator(
    MessageHandlerInterface *component);

/**
 * @brief Creates message handler that throws runtime exception.
 */
std::unique_ptr<MessageHandlerInterface> CreateThrowingMessageHandler();

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_MESSAGE_HANDLER_FACTORY_H_
