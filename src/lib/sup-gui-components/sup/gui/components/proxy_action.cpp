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

#include "proxy_action.h"

namespace
{
const QString kDefaultName = "Proxy";
}

namespace sup::gui
{

ProxyAction::ProxyAction(QObject *parent) : QAction(kDefaultName, parent) {}

QAction *ProxyAction::GetAction() const
{
  return m_action;
}

void ProxyAction::SetAction(QAction *action, flags_t proxy_options)
{
  if (action == m_action)
  {
    return;
  }

  SetConnected(false);

  m_action = action;
  m_proxy_options = proxy_options;

  Update();
  SetConnected(true);
}
void ProxyAction::SetConnected(bool value)
{
  if (!m_action)
  {
    return;
  }

  if (value)
  {
    (void) connect(this, &ProxyAction::triggered, m_action, &QAction::triggered, Qt::UniqueConnection);
    (void) connect(m_action, &QAction::changed, this, &ProxyAction::Update, Qt::UniqueConnection);
  }
  else
  {
    (void) disconnect(this, &ProxyAction::triggered, m_action, &QAction::triggered);
    (void) disconnect(m_action, &QAction::changed, this, &ProxyAction::Update);
  }
}

void ProxyAction::Update()
{
  if (m_action)
  {
    setText(m_action->text());
    setToolTip(m_action->toolTip());
    setIcon(m_action->icon());
    if (m_proxy_options.HasFlag(Options::SyncEnabledStatus))
    {
      setEnabled(m_action->isEnabled());
    }
  }
  else
  {
    setText(kDefaultName);
    setEnabled(false);
    setToolTip("");
  }
}

}  // namespace sup::gui
