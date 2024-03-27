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

#include "proxy_action.h"

namespace
{
const QString kDefaultName = "Proxy";
}

namespace sup::gui
{

ProxyAction::ProxyAction(QObject *parent) : QAction(kDefaultName, parent)
{
  setEnabled(false);
}

QAction *ProxyAction::GetAction() const
{
  return m_action;
}

void ProxyAction::SetAction(QAction *action)
{
  if (action == m_action)
  {
    return;
  }

  if (m_action)
  {
    MakeDisconnected(m_action);
  }

  m_action = action;

  if (m_action)
  {
    Update();
    MakeConnected(m_action);
  }
  else
  {
    setText(kDefaultName);
  }
}

void ProxyAction::MakeDisconnected(QAction *action)
{
  disconnect(this, &ProxyAction::triggered, action, &QAction::triggered);
  disconnect(action, &QAction::changed, this, &ProxyAction::Update);
}

void ProxyAction::MakeConnected(QAction *action)
{
  connect(this, &ProxyAction::triggered, action, &QAction::triggered, Qt::UniqueConnection);
  connect(action, &QAction::changed, this, &ProxyAction::Update, Qt::UniqueConnection);
}

void ProxyAction::Update()
{
  setText(m_action->text());
  setToolTip(m_action->toolTip());
  setEnabled(m_action->isEnabled());
}

}  // namespace sup::gui
