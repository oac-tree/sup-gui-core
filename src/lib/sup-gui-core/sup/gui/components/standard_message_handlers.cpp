/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "standard_message_handlers.h"

#include <sup/gui/core/exceptions.h>

#include <QDebug>

namespace sup::gui
{

void StdMessageHandler::SendMessage(const std::string &text)
{
  qInfo("%s", text.c_str());
}

void NullMessageHandler::SendMessage(const std::string &text)
{
  throw RuntimeException(text);
}

}  // namespace sup::gui
