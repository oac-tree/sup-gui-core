/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef SUP_GUI_WIDGETS_MESSAGE_HELPER_H_
#define SUP_GUI_WIDGETS_MESSAGE_HELPER_H_

//! @file
//! Collection of helper functions to send messages.

#include <sup/gui/core/message_event.h>

#include <QMessageBox>

namespace sup::gui
{

/**
 * @brief Send the message via the QMessageBox.
 */
void SendMessage(const MessageEvent& event, QMessageBox::Icon icon_type);

/**
 * @brief Send warning message via the QMessageBox.
 */
void SendWarningMessage(const MessageEvent& event);

/**
 * @brief Send info message via the QMessageBox.
 */
void SendInfoMessage(const MessageEvent& event);

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_MESSAGE_HELPER_H_
