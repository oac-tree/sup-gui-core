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

#include "app_command.h"

#include <sup/gui/components/proxy_action.h>
#include <sup/gui/core/exceptions.h>

#include <QKeySequence>

namespace sup::gui
{

AppCommand::AppCommand(const QString &text, QObject *parent)
    : AppCommand(text, QKeySequence(), parent)
{
}

AppCommand::AppCommand(const QString &text, const QKeySequence &key, QObject *parent)
    : QObject(parent), m_proxy_action(new ProxyAction(this)), m_default_text(text)
{
  m_proxy_action->setText(text);
  m_proxy_action->setShortcut(key);
  m_proxy_action->setEnabled(false);
}

AppCommand::~AppCommand() = default;

ProxyAction *AppCommand::GetProxyAction()
{
  return m_proxy_action;
}

void AppCommand::SetCurrentContext(const AppContext &current_context)
{
  SetContextStack({current_context});
}

void AppCommand::SetContextStack(const std::vector<AppContext> &context_stack)
{
  for (const auto &current_context : context_stack)
  {
    if (auto action = GetActionForContext(current_context); action)
    {
      m_proxy_action->SetAction(action, ProxyAction::Options::SyncEnabledStatus);
      return;
    }
  }

  m_proxy_action->SetAction(nullptr);
  m_proxy_action->setText(m_default_text);
}

void AppCommand::AddOverrideAction(const AppContext &context, QAction *action)
{
  auto iter = m_context_to_action.find(context);
  if (iter != m_context_to_action.end())
  {
    throw RuntimeException("Already registered context [" + context.GetContextId().toStdString()
                           + "]");
  }

  m_context_to_action.insert(iter, {context, action});
}

AppCommand &AppCommand::SetText(const QString &text)
{
  m_default_text = text;
  m_proxy_action->setText(m_default_text);
  return *this;
}

AppCommand &AppCommand::SetShortcut(const QKeySequence &shortcut)
{
  m_proxy_action->setShortcut(shortcut);
  return *this;
}

bool AppCommand::HasAction(const AppContext &context) const
{
  return m_context_to_action.find(context) != m_context_to_action.end();
}

QAction *AppCommand::GetActionForContext(const AppContext &context) const
{
  auto iter = m_context_to_action.find(context);
  return iter == m_context_to_action.end() ? nullptr : iter->second;
}

}  // namespace sup::gui
