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

#include "app_context.h"

namespace sup::gui
{

AppContext::AppContext() = default;

AppContext::AppContext(const QString &context_id, const QString &context_name)
    : m_context_id(context_id), m_context_name(context_name)
{
}

QString AppContext::GetContextId() const
{
  return m_context_id;
}

QString AppContext::GetContextName() const
{
  return m_context_name;
}

bool AppContext::operator==(const AppContext &other) const
{
  return m_context_id == other.m_context_id;
}

bool AppContext::operator!=(const AppContext &other) const
{
  return !(*this == other);
}

bool AppContext::operator<(const AppContext &other) const
{
  return m_context_id < other.m_context_id;
}

bool AppContext::operator>=(const AppContext &other) const
{
  return (other < *this) || (*this == other);
}

}  // namespace sup::gui
