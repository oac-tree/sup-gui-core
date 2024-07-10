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

#include "custom_header_view.h"

#include <QSettings>

namespace sup::gui
{

CustomHeaderView::CustomHeaderView(QWidget *parent) : CustomHeaderView({}, parent) {}

CustomHeaderView::~CustomHeaderView()
{
  WriteSettings();
}

CustomHeaderView::CustomHeaderView(const QString &setting_group_name, QWidget *parent)
    : QHeaderView(Qt::Horizontal, parent)
{
  setDefaultAlignment(Qt::AlignLeft);
  connect(this, &QHeaderView::sectionResized, this, &CustomHeaderView::OnSectionResize);

  ReadSettings();
}

void CustomHeaderView::SetAsFavoriteState(QByteArray state)
{
  // we set favorite state, but do not use it yet
  m_favorite_state = std::move(state);
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
  }
}

void CustomHeaderView::mousePressEvent(QMouseEvent *event)
{
  m_is_in_interactive_mode = true;
  QHeaderView::mousePressEvent(event);
}

void CustomHeaderView::mouseReleaseEvent(QMouseEvent *event)
{
  m_is_in_interactive_mode = false;
  QHeaderView::mouseReleaseEvent(event);
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

  // we read favorite state, but do not use it yet
}

void CustomHeaderView::WriteSettings()
{
  if (m_setting_name.isEmpty())
  {
    return;
  }

  QSettings settings;
  if (HasFavoriteState())
  {
    settings.setValue(m_setting_name, GetFavoriteState());
  }
}

void CustomHeaderView::OnSectionResize(int index, int prev_size, int new_size)
{
  if (!m_is_in_interactive_mode)
  {
    return;
  }

  m_favorite_state = saveState();
}

}  // namespace sup::gui
