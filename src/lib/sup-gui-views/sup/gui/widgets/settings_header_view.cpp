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

#include "settings_header_view.h"

#include <sup/gui/components/tree_helper.h>
#include <sup/gui/settings/i_settings_manager.h>

#include <QAction>
#include <QMenu>
#include <QMouseEvent>

namespace sup::gui
{

SettingsHeaderView::SettingsHeaderView(std::string group, ISettingsManager& settings_manager,
                                       std::vector<int> stretch_factors, QWidget* parent_widget)
    : QHeaderView(Qt::Horizontal, parent_widget)
    , m_group(std::move(group))
    , m_settings_manager(settings_manager)
    , m_stretch_factors(std::move(stretch_factors))
{
  setDefaultAlignment(Qt::AlignLeft);
  connect(this, &QHeaderView::sectionResized, this, &SettingsHeaderView::OnSectionResize);

  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &SettingsHeaderView::customContextMenuRequested, this,
          &SettingsHeaderView::OnContextMenuRequest);

  m_favorite_state = m_settings_manager.Get<HeaderInfo>(m_group, SettingsScope::kApplication).state;
  m_provider_connection = m_settings_manager.RegisterProvider(m_group, SettingsScope::kApplication,
                                                              [this]() { return GetHeaderInfo(); });
}

HeaderInfo SettingsHeaderView::GetHeaderInfo() const
{
  if (m_interaction_state.IsSet(InteractionState::kWasAdjusted))
  {
    return {m_favorite_state};
  }
  return m_settings_manager.Get<HeaderInfo>(m_group, SettingsScope::kApplication);
}

void SettingsHeaderView::AdjustColumnsWidth()
{
  if (!m_favorite_state.isEmpty())
  {
    (void)restoreState(m_favorite_state);
    return;
  }

  if (!m_stretch_factors.empty())
  {
    AdjustWidthOfColumns(this, m_stretch_factors);
    return;
  }

  const std::vector<int> stretch_factors(count(), 1);
  AdjustWidthOfColumns(this, stretch_factors);
}

void SettingsHeaderView::ResetColumnWidth()
{
  m_interaction_state.Set(InteractionState::kWasAdjusted);
  m_favorite_state.clear();
  AdjustColumnsWidth();
}

void SettingsHeaderView::mousePressEvent(QMouseEvent* event)
{
  // the right button opens the context menu and doesn't resize
  if (event->button() == Qt::LeftButton)
  {
    m_interaction_state.Set(InteractionState::kInteractiveMode);
  }
  QHeaderView::mousePressEvent(event);
}

void SettingsHeaderView::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
  {
    m_interaction_state.Reset(InteractionState::kInteractiveMode);
  }
  QHeaderView::mouseReleaseEvent(event);
}

void SettingsHeaderView::showEvent(QShowEvent* event)
{
  Q_UNUSED(event)

  // the tree has its physical size only on the first show
  if (!m_interaction_state.IsSet(InteractionState::kWasShown))
  {
    AdjustColumnsWidth();
    m_interaction_state.Set(InteractionState::kWasShown);
  }
}

void SettingsHeaderView::OnContextMenuRequest(const QPoint& point)
{
  QMenu menu;

  auto reset_action = menu.addAction("Reset column width to optimal");
  connect(reset_action, &QAction::triggered, this, [this]() { ResetColumnWidth(); });

  (void)menu.exec(mapToGlobal(point));
}

void SettingsHeaderView::OnSectionResize(int index, int prev_size, int new_size)
{
  Q_UNUSED(index);
  Q_UNUSED(prev_size);
  Q_UNUSED(new_size);

  if (m_interaction_state.IsSet(InteractionState::kInteractiveMode))
  {
    m_interaction_state.Set(InteractionState::kWasAdjusted);
    m_favorite_state = saveState();
  }
}

}  // namespace sup::gui
