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

#include "app_context.h"

namespace sup::gui
{

AppContext::AppContext() = default;

AppContext::AppContext(const QString &context, QWidget *focus_widget)
    : m_context_name(context), m_focus_widget(focus_widget)
{
}

QString AppContext::GetContextName() const
{
  return m_context_name;
}

QWidget *AppContext::GetFocusWidget() const
{
  return m_focus_widget;
}

bool AppContext::operator==(const AppContext &other) const
{
  return m_context_name == other.m_context_name && m_focus_widget == other.m_focus_widget;
}

bool AppContext::operator!=(const AppContext &other) const
{
  return !(*this == other);
}

}  // namespace sup::gui
