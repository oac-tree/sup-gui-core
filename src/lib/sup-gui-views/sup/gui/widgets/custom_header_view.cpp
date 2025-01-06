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

#include "custom_header_view.h"

#include <sup/gui/components/tree_helper.h>

#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include <QSettings>

namespace sup::gui
{

CustomHeaderView::CustomHeaderView(QWidget *parent) : CustomHeaderView({}, {}, parent) {}

CustomHeaderView::CustomHeaderView(const QString &setting_name, QWidget *parent)
    : CustomHeaderView(setting_name, {}, parent)
{
}

CustomHeaderView::CustomHeaderView(const QString &setting_name,
                                   const std::vector<int> &stretch_factors, QWidget *parent)
    : QHeaderView(Qt::Horizontal, parent)
    , m_setting_name(setting_name)
    , m_stretch_factors(stretch_factors)
{
  setDefaultAlignment(Qt::AlignLeft);
  connect(this, &QHeaderView::sectionResized, this, &CustomHeaderView::OnSectionResize);

  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &CustomHeaderView::customContextMenuRequested, this,
          &CustomHeaderView::OnContextMenuRequest);

  ReadSettings();
}

CustomHeaderView::~CustomHeaderView()
{
  WriteSettings();
}

void CustomHeaderView::SetAsFavoriteState(const QByteArray &state)
{
  // we set favorite state, but do not use it yet
  m_favorite_state = state;
}

void CustomHeaderView::RestoreFavoriteState()
{
  if (!m_favorite_state.isEmpty())
  {
    restoreState(m_favorite_state);
  }
}

bool CustomHeaderView::HasFavoriteState() const
{
  return !m_favorite_state.isEmpty();
}

QByteArray CustomHeaderView::GetFavoriteState() const
{
  return m_favorite_state;
}

void CustomHeaderView::AdjustColumnsWidth()
{
  if (HasFavoriteState())
  {
    RestoreFavoriteState();
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

void CustomHeaderView::ResetColumnWidth()
{
  m_interaction_state.SetFlag(InteractionState::kWasAdjusted);
  m_favorite_state.clear();
  AdjustColumnsWidth();
}

void CustomHeaderView::mousePressEvent(QMouseEvent *event)
{
  // Catching when the user has started to resize columns. Ignoring press events for context
  // right-mouse button event.
  if (event->buttons() & Qt::LeftButton)
  {
    m_interaction_state.SetFlag(InteractionState::kInteractiveMode);
  }
  QHeaderView::mousePressEvent(event);
}

void CustomHeaderView::mouseReleaseEvent(QMouseEvent *event)
{
  // Catching when the user has stopped to resize columns. Ignoring release event for context
  // right-mouse button event.
  if (event->button() == Qt::LeftButton)
  {
    m_interaction_state.UnsetFlag(InteractionState::kInteractiveMode);
  }
  QHeaderView::mouseReleaseEvent(event);
}

void CustomHeaderView::showEvent(QShowEvent *event)
{
  Q_UNUSED(event)

  if (!m_interaction_state.HasFlag(InteractionState::kWasShown))
  {
    // make column adjustment on the first show event, when tree got its physical size
    // do not try to adjust on the next show events
    AdjustColumnsWidth();
    m_interaction_state.SetFlag(InteractionState::kWasShown);
  }
}

void CustomHeaderView::OnContextMenuRequest(const QPoint &point)
{
  QMenu menu;

  auto reset_action = menu.addAction("Reset column width to optimal");
  connect(reset_action, &QAction::triggered, this, [this]() { ResetColumnWidth(); });

  menu.exec(mapToGlobal(point));
}

void CustomHeaderView::ReadSettings()
{
  if (m_setting_name.isEmpty())
  {
    return;
  }

  const QSettings settings;

  if (settings.contains(m_setting_name))
  {
    SetAsFavoriteState(settings.value(m_setting_name).toByteArray());
  }
}

void CustomHeaderView::WriteSettings()
{
  if (m_setting_name.isEmpty())
  {
    return;
  }

  if (m_interaction_state.HasFlag(InteractionState::kWasAdjusted))
  {
    // This covers a case of two widgets looking at the same setting name. If the widget was never
    // adjusted by the user, it will never try to overwrite the setting. However, if two widgets
    // with the same setting name were adjusted by the user, the widget that was destructed the
    // last, will overwrite the settings of the previous widget.

    QSettings settings;
    settings.setValue(m_setting_name, GetFavoriteState());
  }
}

void CustomHeaderView::OnSectionResize(int index, int prev_size, int new_size)
{
  Q_UNUSED(index);
  Q_UNUSED(prev_size);
  Q_UNUSED(new_size);

  if (m_interaction_state.HasFlag(InteractionState::kInteractiveMode))
  {
    m_interaction_state.SetFlag(InteractionState::kWasAdjusted);
    m_favorite_state = saveState();
  }
}

}  // namespace sup::gui
