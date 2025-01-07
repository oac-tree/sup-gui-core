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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sup/gui/widgets/collapsible_toolbar.h"

#include <sup/gui/style/style_helper.h>

#include <mvvm/widgets/widget_utils.h>

#include <QLabel>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

namespace sup::gui
{

CollapsibleToolBar::CollapsibleToolBar(QWidget *parent_widget)
    : QFrame(parent_widget)
    , m_tool_bar(new QToolBar)
    , m_expand_button(new QToolButton)
    , m_label(new QLabel)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tool_bar);

  m_tool_bar->setIconSize(utils::NarrowToolBarIconSize());

  // make it narrow, buttons closer to each other
  m_tool_bar->setStyleSheet(
      "QToolBar { padding-bottom: 0px; margin-bottom: 0px; } QToolButton { padding: 0px; margin: "
      "0px; }");

  m_expand_button->setIcon(utils::FindIcon("chevron-down"));
  auto on_toggle_expand = [this]() { SetExpanded(!m_is_expanded); };
  connect(m_expand_button, &QToolButton::clicked, this, on_toggle_expand);
  m_tool_bar->addWidget(m_expand_button);

  m_tool_bar->addWidget(m_label);
  InsertStrech();

  setFixedHeight(utils::NarrowToolBarIconSize().height() * 1.5);

  UpdateToolBar();
}

void CollapsibleToolBar::SetText(const QString &text, const QString &text_tooltip)
{
  mvvm::utils::ScaleLabelFont(m_label, 0.9);
  m_label->setText(text);
  m_label->setToolTip(text_tooltip);
}

void CollapsibleToolBar::SetControlledWidget(QWidget *widget)
{
  m_controlled_widget = widget;
}

void CollapsibleToolBar::AddWidget(QWidget *widget)
{
  // Feature of QToolBar: returns action on adding any widget or button.
  // These actions can be used to hide widgets from the toolbar (see ::UpdateToolBar)
  auto action = m_tool_bar->addWidget(widget);
  m_toolbar_actions.append(action);  // to hide/show elements later
}

void CollapsibleToolBar::AddActions(const QList<QAction *> &actions)
{
  for (auto action : actions)
  {
    m_tool_bar->addAction(action);
    m_toolbar_actions.append(action);
  }
}

QToolBar *CollapsibleToolBar::GetToolBar()
{
  return m_tool_bar;
}

void CollapsibleToolBar::SetExpanded(bool value)
{
  m_is_expanded = value;
  UpdateToolBar();
}

void CollapsibleToolBar::UpdateToolBar()
{
  UpdateIcon();

  // show/hide controlled widget
  if (m_controlled_widget)
  {
    m_controlled_widget->setVisible(m_is_expanded);
  }

  // show/hide toolbar elements
  for (auto action : m_toolbar_actions)
  {
    action->setVisible(m_is_expanded);
  }
}

void CollapsibleToolBar::UpdateIcon()
{
  if (m_is_expanded)
  {
    m_expand_button->setIcon(utils::FindIcon("chevron-down"));
    setFrameStyle(QFrame::StyledPanel);
  }
  else
  {
    m_expand_button->setIcon(utils::FindIcon("chevron-right"));
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
  }
}

void CollapsibleToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  m_tool_bar->addWidget(empty);
}

}  // namespace sup::gui
