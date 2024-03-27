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

  SetConnected(false);

  m_action = action;

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
    connect(this, &ProxyAction::triggered, m_action, &QAction::triggered, Qt::UniqueConnection);
    connect(m_action, &QAction::changed, this, &ProxyAction::Update, Qt::UniqueConnection);
  }
  else
  {
    disconnect(this, &ProxyAction::triggered, m_action, &QAction::triggered);
    disconnect(m_action, &QAction::changed, this, &ProxyAction::Update);
  }
}

void ProxyAction::Update()
{
  if (m_action)
  {
    setText(m_action->text());
    setToolTip(m_action->toolTip());
    setEnabled(m_action->isEnabled());
  }
  else
  {
    setText(kDefaultName);
    setToolTip("");
    setEnabled(false);
  }
}

}  // namespace sup::gui
