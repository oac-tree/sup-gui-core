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

#ifndef SUP_GUI_COMPONENTS_MESSAGE_EVENT_H_
#define SUP_GUI_COMPONENTS_MESSAGE_EVENT_H_

#include <string>

namespace sup::gui
{

/**
 * @brief The MessageEvent struct is a simple aggregate to send messages to the user as a reaction
 * to his interactions with UI elements.
 */
struct MessageEvent
{
  //!< A message title (i.e. the title of message dialog).
  std::string title;

  //!< A primary text to alert the user of the situation ("document was modified").
  std::string text;

  //!< Further explanations or may be a question ("do you want to save").
  std::string informative;

  //!< Optional details that can be shown on the request (exception's what()).
  std::string detailed;

  bool operator==(const MessageEvent& other) const;
  bool operator!=(const MessageEvent& other) const;
};

MessageEvent CreateInvalidOperationMessage(const std::string& text,
                                           const std::string& informative = {},
                                           const std::string& details = {});

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_MESSAGE_EVENT_H_
