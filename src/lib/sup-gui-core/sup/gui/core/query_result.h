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

#ifndef SUP_GUI_CORE_QUERY_RESULT_H_
#define SUP_GUI_CORE_QUERY_RESULT_H_

#include <sup/gui/core/message_event.h>

namespace sup::gui
{

/**
 * @brief The QueryResult class is a wrapper around MessageEvent to carry a flag denoting the
 * success of the operation.
 *
 * Unsuccessfull operation normally means that the user notification is necessary.
 */
class QueryResult
{
public:
  enum class Status
  {
    kSuccess,
    kFailure
  };

  QueryResult() = delete;

  /**
   * @brief Checks if query result was successfull.
   */
  bool IsSuccess() const;

  /**
   * @brief A factory method to build an object carrying a failure flag, and the message.
   */
  static QueryResult Failure(const sup::gui::MessageEvent& message);

  /**
   * @brief A factory method to build an object carrying a success flag.
   */
  static QueryResult Success();

  /**
   * @brief Returns onboard message.
   */
  sup::gui::MessageEvent GetMessage() const;

private:
  QueryResult(Status status, sup::gui::MessageEvent message);

  Status m_status{Status::kSuccess};
  sup::gui::MessageEvent m_message;
};

}  // namespace sup::gui

#endif  // SUP_GUI_CORE_QUERY_RESULT_H_
