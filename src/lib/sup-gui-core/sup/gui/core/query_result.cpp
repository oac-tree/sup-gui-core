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

#include "query_result.h"

namespace sup::gui
{

QueryResult::QueryResult(Status status, sup::gui::MessageEvent message)
    : m_status(status), m_message(std::move(message))
{
}

bool QueryResult::IsSuccess() const
{
  return m_status == Status::kSuccess;
}

QueryResult QueryResult::Failure(const sup::gui::MessageEvent &message)
{
  return {Status::kFailure, message};
}

QueryResult QueryResult::Success()
{
  return {Status::kSuccess, {}};
}

sup::gui::MessageEvent QueryResult::GetMessage() const
{
  return m_message;
}

}  // namespace sup::gui
