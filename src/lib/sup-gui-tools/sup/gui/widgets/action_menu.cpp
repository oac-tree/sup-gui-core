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

#include "action_menu.h"

#include <QToolBar>
#include <QToolButton>

namespace sup::gui
{

ActionMenu::ActionMenu(QObject *parent) : QWidgetAction(parent) {}

ActionMenu::ActionMenu(const QString &text, QObject *parent) : ActionMenu(parent)
{
  setText(text);
}

ActionMenu::ActionMenu(const QIcon &icon, const QString &text, QObject *parent)
    : ActionMenu(text, parent)
{
  setIcon(icon);
}

QWidget *ActionMenu::createWidget(QWidget *parent)
{
  auto toolbar = qobject_cast<QToolBar *>(parent);
  if (!toolbar)
  {
    return QWidgetAction::createWidget(parent);
  }

  // when inserted in a toolbar, we create a button and let the toolbar show it, instead of us
  auto button = new QToolButton(toolbar);
  button->setAutoRaise(true);
  button->setFocusPolicy(Qt::NoFocus);
  button->setIconSize(toolbar->iconSize());
  button->setToolButtonStyle(toolbar->toolButtonStyle());
  connect(toolbar, &QToolBar::iconSizeChanged, button, &QAbstractButton::setIconSize);
  connect(toolbar, &QToolBar::toolButtonStyleChanged, button, &QToolButton::setToolButtonStyle);

  // click on the button will immediately bring up our own menu
  button->setDefaultAction(this);
  button->setPopupMode(QToolButton::InstantPopup);
  connect(button, &QToolButton::triggered, toolbar, &QToolBar::actionTriggered);

  return button;
}

}  // namespace sup::gui
