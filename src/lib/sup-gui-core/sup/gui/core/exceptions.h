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

#ifndef SUP_GUI_CORE_EXCEPTIONS_H_
#define SUP_GUI_CORE_EXCEPTIONS_H_

#include <exception>
#include <string>

namespace sup::gui
{

//! Base Exception class with message.

class MessageException : public std::exception
{
public:
  explicit MessageException(std::string message);
  const char* what() const noexcept override;

private:
  std::string message;
};

//! Exception is thrown on invalid operation.

class NotImplementedException : public MessageException
{
public:
  explicit NotImplementedException(const std::string& message);
};

//! Generic exception to throw at runtime.

class RuntimeException : public MessageException
{
public:
  explicit RuntimeException(const std::string& message);
};

//! Generic exception to throw as a consequence of faulty logic (viloated preconditions etc).

class LogicErrorException : public MessageException
{
public:
  explicit LogicErrorException(const std::string& message);
};

//! Exception is thrown when the argument is nullptr.

class NullException : public MessageException
{
public:
  explicit NullException(const std::string& message);
};

//! Exception is thrown on invalid operation.

class InvalidOperationException : public MessageException
{
public:
  explicit InvalidOperationException(const std::string& message);
};

}  // namespace sup::gui

#endif  // SUP_GUI_CORE_EXCEPTIONS_H_
