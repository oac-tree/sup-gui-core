/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_COMPONENTS_MESSAGE_HANDLER_INTERFACE_H_
#define SUP_GUI_COMPONENTS_MESSAGE_HANDLER_INTERFACE_H_

#include <string>

namespace sup::gui
{

//! Interface class to send messages to the user.
class MessageHandlerInterface
{
public:
  virtual ~MessageHandlerInterface() = default;
  virtual void SendMessage(const std::string& text) = 0;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_MESSAGE_HANDLER_INTERFACE_H_
