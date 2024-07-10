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

#include "tree_helper.h"

#include <QAction>
#include <QMenu>
#include <QSettings>

namespace sup::gui
{

CustomHeaderView::CustomHeaderView(QWidget *parent) : CustomHeaderView({}, {}, parent) {}

CustomHeaderView::~CustomHeaderView()
{
  WriteSettings();
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

bool CustomHeaderView::AdjustColumnsWidth()
{
  if (HasFavoriteState())
  {
    RestoreFavoriteState();
    return true;
  }

  if (!m_stretch_factors.empty())
  {
    AdjustWidthOfColumns(this, m_stretch_factors);
    return true;
  }

  return false;
}

void CustomHeaderView::ResetColumnWidth()
{
  SetAsFavoriteState({});

  if (!m_stretch_factors.empty())
  {
    AdjustWidthOfColumns(this, m_stretch_factors);
    return;
  }

  // evenly distribute column width
  const std::vector<int> stretch_factors(1, count());
  AdjustWidthOfColumns(this, stretch_factors);
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
