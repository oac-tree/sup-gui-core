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

#include "panel_toolbar.h"

#include <sup/gui/widgets/style_utils.h>

#include <QLabel>
#include <QToolButton>

namespace sup::gui
{

PanelToolBar::PanelToolBar(QWidget *parent) : QToolBar(parent), m_label(new QLabel)
{
  setIconSize(sup::gui::utils::ToolBarIconSize());
  //  setStyleSheet("QToolButton:!hover {background-color:#eff6fc} QToolBar {background: #eff6fc}");

  addWidget(m_label);

  AddDotsMenu();
}

void PanelToolBar::setText(const QString &text)
{
  m_label->setText(text);
}

void PanelToolBar::AddDotsMenu()
{
  InsertStrech();

  m_dots_button = new QToolButton;
  m_dots_button->setIcon(utils::GetIcon("dots-vertical"));
  m_dots_button->setIconSize(sup::gui::utils::ToolBarIconSize());
  addWidget(m_dots_button);
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

//! Sets a menu to the button with dots. The menu ownership remains on caller side.

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

}  // namespace sup::gui
