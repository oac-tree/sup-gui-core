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

#include "exceptions.h"

namespace sup::gui
{

MessageException::MessageException(std::string message) : m_message{std::move(message)} {}

const char* MessageException::what() const noexcept
{
  return m_message.c_str();
}

NotImplementedException::NotImplementedException(const std::string& message)
    : MessageException{message}
{
}

RuntimeException::RuntimeException(const std::string& message) : MessageException{message} {}

LogicErrorException::LogicErrorException(const std::string& message) : MessageException{message} {}

NullException::NullException(const std::string& message) : MessageException{message} {}

InvalidOperationException::InvalidOperationException(const std::string& message)
    : MessageException{message}
{
}

}  // namespace sup::gui
