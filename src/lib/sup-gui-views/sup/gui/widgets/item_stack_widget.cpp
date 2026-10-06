/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "item_stack_widget.h"

#include "panel_toolbar.h"

#include <sup/gui/style/style_helper.h>

#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QStackedWidget>
#include <QToolBar>
#include <QVBoxLayout>

namespace sup::gui
{

ItemStackWidget::ItemStackWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_stacked_widget(new QStackedWidget)
    , m_widget_selection_menu(std::make_unique<QMenu>())
    , m_main_toolbar(new PanelToolBar)
    , m_action_group(new QActionGroup(this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_main_toolbar);
  layout->addWidget(m_stacked_widget);

  m_main_toolbar->SetDotsMenu(m_widget_selection_menu.get());

  SetPanelStyle(PanelStyle::kButtonTextBesideIcon);
}

void ItemStackWidget::SetPanelStyle(PanelStyle panel_style)
{
  if (panel_style == PanelStyle::kIconOnly)
  {
    m_main_toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_main_toolbar->setIconSize(utils::ToolBarIconSize());
  }
  else if (panel_style == PanelStyle::kIconOnly)
  {
    m_main_toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  }
  else if (panel_style == PanelStyle::kButtonTextUnderIcon)
  {
    m_main_toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_main_toolbar->setIconSize(utils::NarrowToolBarIconSize());
  }
  else
  {
    // nothing to do
  }
}

void ItemStackWidget::AddWidget(QWidget* widget)
{
  AddWidget(widget, {});
}

void ItemStackWidget::AddWidget(QWidget* widget, const QList<QAction*>& actions,
                                bool is_always_visible)
{
  m_stacked_widget->addWidget(widget);
  AddMenuEntry(widget);
  AddGuestActions(actions, is_always_visible);
}

std::int32_t ItemStackWidget::GetCurrentIndex() const
{
  return m_stacked_widget->currentIndex();
}

void ItemStackWidget::SetCurrentIndex(std::int32_t index)
{
  if (index < 0 || index >= m_stacked_widget->count())
  {
    return;
  }
  m_stacked_widget->setCurrentIndex(index);
  m_widget_selection_menu->actions().at(index)->setChecked(true);
  UpdateControlElementsVisibility();
}

QWidget* ItemStackWidget::GetCurrentWidget() const
{
  return m_stacked_widget->currentWidget();
}

void ItemStackWidget::SetCurrentWidget(QWidget* widget)
{
  SetCurrentIndex(m_stacked_widget->indexOf(widget));
}

PanelToolBar* ItemStackWidget::GetToolBar()
{
  return m_main_toolbar;
}

void ItemStackWidget::AddMenuEntry(QWidget* widget)
{
  const std::int32_t index = m_stacked_widget->count() - 1;
  auto action = m_widget_selection_menu->addAction(widget->windowTitle());
  action->setActionGroup(m_action_group);
  action->setCheckable(true);
  if (index == 0)
  {
    action->setChecked(true);
  }
  auto on_action = [this, index]() { SetCurrentIndex(index); };
  connect(action, &QAction::triggered, this, on_action);
}

void ItemStackWidget::AddGuestActions(const QList<QAction*>& actions, bool is_always_visible)
{
  QList<QAction*> hide_actions;  // handles to hide actions

  for (auto action : actions)
  {
    hide_actions.append(m_main_toolbar->InsertElement(action));
  }

  m_toolbar_data.append({hide_actions, is_always_visible});
  UpdateControlElementsVisibility();
}

void ItemStackWidget::UpdateControlElementsVisibility()
{
  for (std::int32_t i = 0; i < m_toolbar_data.size(); ++i)
  {
    const bool is_visible =
        (m_stacked_widget->currentIndex() == i) || m_toolbar_data[i].is_always_visible;
    for (auto action : m_toolbar_data[i].actions)
    {
      action->setVisible(is_visible);
    }
  }
}

}  // namespace sup::gui
