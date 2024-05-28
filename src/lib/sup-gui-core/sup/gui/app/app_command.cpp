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

#include <sup/gui/widgets/proxy_action.h>

#include <QKeySequence>

namespace sup::gui
{

AppCommand::AppCommand(const QString &text, QObject *parent)
    : AppCommand(text, QKeySequence(), parent)
{
}

AppCommand::AppCommand(const QString &text, const QKeySequence &key, QObject *parent)
    : QObject(parent), m_proxy_action(new ProxyAction(this))
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
  for (const auto &[action, context] : m_action_to_context)
  {
    if (current_context == context)
    {
      m_proxy_action->SetAction(action, ProxyAction::Options::SyncEnabledStatus);
      return;
    }
  }

  m_proxy_action->SetAction(nullptr);
}

void AppCommand::AddOverrideAction(const AppContext &context, QAction *action)
{
  m_action_to_context[action] = context;
}

void AppCommand::SetKeySequence(const QKeySequence &shortcut)
{
  m_proxy_action->setShortcut(shortcut);
}

}  // namespace sup::gui
