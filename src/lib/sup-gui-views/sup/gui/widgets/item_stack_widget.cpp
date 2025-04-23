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

#include "item_stack_widget.h"

#include "panel_toolbar.h"

#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QStackedWidget>
#include <QToolBar>
#include <QVBoxLayout>

namespace sup::gui
{

namespace
{

/**
 * @brief Returns settings key to store widget index.
 */
QString GetSettingsKeyForIndex(const QString &settings_group_name)
{
  return settings_group_name + "_index";
}

}  // namespace

ItemStackWidget::ItemStackWidget(QWidget *parent_widget) : ItemStackWidget(QString(), parent_widget)
{
}

ItemStackWidget::ItemStackWidget(const QString &settings_group_name, QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_stacked_widget(new QStackedWidget)
    , m_widget_selection_menu(std::make_unique<QMenu>())
    , m_main_toolbar(new PanelToolBar)
    , m_action_group(new QActionGroup(this))
    , m_settings_group_name(settings_group_name)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_main_toolbar);
  layout->addWidget(m_stacked_widget);

  m_main_toolbar->SetDotsMenu(m_widget_selection_menu.get());
}

ItemStackWidget::~ItemStackWidget() = default;

void ItemStackWidget::AddWidget(QWidget *widget)
{
  m_stacked_widget->addWidget(widget);
  AddMenuEntry(widget);
}

void ItemStackWidget::AddWidget(QWidget *widget, const QList<QAction *> &actions,
                                bool is_always_visible)
{
  AddWidget(widget);
  AddGuestActions(actions, is_always_visible);
}

int ItemStackWidget::GetCurrentIndex() const
{
  return m_stacked_widget->currentIndex();
}

void ItemStackWidget::SetCurrentIndex(int index)
{
  m_stacked_widget->setCurrentIndex(index);
  m_widget_selection_menu->actions().at(index)->setChecked(true);
  UpdateControlElementsVisibility();
}

QWidget *ItemStackWidget::GetCurrentWidget() const
{
  return m_stacked_widget->currentWidget();
}

void ItemStackWidget::SetCurrentWidget(QWidget *widget)
{
  SetCurrentIndex(m_stacked_widget->indexOf(widget));
}

void ItemStackWidget::ReadSettings(const read_variant_func_t &read_func)
{
  if (m_settings_group_name.isEmpty())
  {
    return;
  }

  auto index_variant = read_func(GetSettingsKeyForIndex(m_settings_group_name));
  if (index_variant.isValid())
  {
    SetCurrentIndex(index_variant.toInt());
  }
}

void ItemStackWidget::WriteSettings(const write_variant_func_t &write_func)
{
  if (m_settings_group_name.isEmpty())
  {
    return;
  }

  write_func(GetSettingsKeyForIndex(m_settings_group_name), GetCurrentIndex());
}

void ItemStackWidget::AddMenuEntry(QWidget *widget)
{
  int index = m_stacked_widget->count() - 1;
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

void ItemStackWidget::AddGuestActions(const QList<QAction *> &actions, bool is_always_visible)
{
  QList<QAction *> hide_actions;  // handles to hide actions

  for (auto action : actions)
  {
    hide_actions.append(m_main_toolbar->InsertElement(action));
  }

  m_toolbar_data.append({hide_actions, is_always_visible});
  UpdateControlElementsVisibility();
}

void ItemStackWidget::UpdateControlElementsVisibility()
{
  for (int i = 0; i < m_toolbar_data.size(); ++i)
  {
    bool is_visible = m_stacked_widget->currentIndex() == i || m_toolbar_data[i].is_always_visible;
    for (auto action : m_toolbar_data[i].actions)
    {
      action->setVisible(is_visible);
    }
  }
}

}  // namespace sup::gui
