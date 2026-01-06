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

#include "message_event.h"

namespace sup::gui
{

bool operator==(const MessageEvent& lhs, const MessageEvent& rhs)
{
  return lhs.title == rhs.title && lhs.text == rhs.text && lhs.informative == rhs.informative
         && lhs.detailed == rhs.detailed;
}

bool operator!=(const MessageEvent& lhs, const MessageEvent& rhs)
{
  return !(lhs == rhs);
}

MessageEvent CreateInvalidOperationMessage(const std::string& text, const std::string& informative,
                                           const std::string& details)
{
  return {"Invalid operation", text, informative, details};
}

}  // namespace sup::gui
