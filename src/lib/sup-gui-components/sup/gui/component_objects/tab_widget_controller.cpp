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

#include "tab_widget_controller.h"

#include <QTabWidget>
#include <algorithm>

namespace sup::gui
{

TabWidgetController::TabWidgetController(QObject* parent_object) : QObject(parent_object) {}

TabWidgetController::~TabWidgetController() = default;

void TabWidgetController::AddTabWidget(QTabWidget* tab_widget)
{
  if (!tab_widget
      || std::find(m_tab_widgets.begin(), m_tab_widgets.end(), tab_widget) != m_tab_widgets.end())
  {
    return;
  }

  m_tab_widgets.push_back(tab_widget);

  connect(tab_widget, &QTabWidget::currentChanged, this,
          [this, tab_widget](int index) { OnCurrentChanged(tab_widget, index); });

  // Remove from list automatically when the widget is destroyed.
  connect(tab_widget, &QObject::destroyed, this,
          [this, tab_widget]()
          {
            m_tab_widgets.erase(std::remove(m_tab_widgets.begin(), m_tab_widgets.end(), tab_widget),
                                m_tab_widgets.end());
          });
}

void TabWidgetController::RemoveTabWidget(QTabWidget* tab_widget)
{
  auto it = std::find(m_tab_widgets.begin(), m_tab_widgets.end(), tab_widget);
  if (it == m_tab_widgets.end())
  {
    return;
  }

  m_tab_widgets.erase(it);

  disconnect(tab_widget, nullptr, this, nullptr);
}

void TabWidgetController::SetActive(bool value)
{
  const bool reactivated = value && !m_is_active;
  m_is_active = value;

  if (reactivated)
  {
    SyncToFirstWidget();
  }
}

bool TabWidgetController::IsActive() const
{
  return m_is_active;
}

void TabWidgetController::OnCurrentChanged(QTabWidget* source, int index)
{
  if (!m_is_active || m_is_propagating)
  {
    return;
  }

  m_is_propagating = true;
  for (auto* tab_widget : m_tab_widgets)
  {
    if (tab_widget != source && index < tab_widget->count())
    {
      tab_widget->setCurrentIndex(index);
    }
  }
  m_is_propagating = false;
}

void TabWidgetController::SyncToFirstWidget()
{
  if (m_tab_widgets.empty())
  {
    return;
  }

  const int index = m_tab_widgets.front()->currentIndex();
  OnCurrentChanged(m_tab_widgets.front(), index);
}

}  // namespace sup::gui
