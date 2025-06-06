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

#include "panel_toolbar.h"

#include <sup/gui/style/style_helper.h>

#include <QLabel>
#include <QToolButton>

namespace sup::gui
{

PanelToolBar::PanelToolBar(QWidget *parent_widget) : QToolBar(parent_widget), m_label(new QLabel)
{
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setIconSize(sup::gui::utils::ToolBarIconSize());
  //  setStyleSheet("QToolButton:!hover {background-color:#eff6fc} QToolBar {background: #eff6fc}");

  m_label_action = addWidget(m_label);
  m_label_action->setVisible(false);

  SetupDotsButton();
}

void PanelToolBar::SetText(const QString &text)
{
  m_label_action->setVisible(true);
  m_label->setText(text);
}

QAction *PanelToolBar::InsertElement(QWidget *widget)
{
  return insertWidget(m_spacer_action, widget);
}

QAction *PanelToolBar::InsertElement(QAction *action)
{
  insertAction(m_spacer_action, action);
  return action;
}

void PanelToolBar::SetDotsMenu(QMenu *dots_menu)
{
  m_dots_button->setMenu(dots_menu);
  m_dots_button->setPopupMode(QToolButton::InstantPopup);
}

QAction *PanelToolBar::AppendSeparator()
{
  return insertSeparator(m_spacer_action);
}

void PanelToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  m_spacer_action = addWidget(empty);
}

void PanelToolBar::SetupDotsButton()
{
  InsertStrech();

  m_dots_button = new QToolButton;
  m_dots_button->setIcon(utils::FindIcon("dots-vertical"));
  m_dots_button->setIconSize(sup::gui::utils::ToolBarIconSize());
  addWidget(m_dots_button);
}

}  // namespace sup::gui
