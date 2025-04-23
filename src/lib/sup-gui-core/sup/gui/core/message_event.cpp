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

#include "message_event.h"

namespace sup::gui
{

bool MessageEvent::operator==(const MessageEvent& other) const
{
  return title == other.title && text == other.text && informative == other.informative
         && detailed == other.detailed;
}

bool MessageEvent::operator!=(const MessageEvent& other) const
{
  return !(*this == other);
}

MessageEvent CreateInvalidOperationMessage(const std::string& text, const std::string& informative,
                                           const std::string& details)
{
  return {"Invalid operation", text, informative, details};
}

}  // namespace sup::gui
