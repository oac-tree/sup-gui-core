/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

namespace sup::gui
{

CustomHeaderView::CustomHeaderView(QWidget *parent) : QHeaderView(Qt::Horizontal, parent)
{
  connect(this, &QHeaderView::sectionResized, this, &CustomHeaderView::OnSectionResize);
}

void CustomHeaderView::SetAsFavoriteState(QByteArray state)
{
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

//! Save size of columns in a header on any interactive resize activity. Will ignore programmatic
//! column resize (i.e. due to resize of the parent widget).
void CustomHeaderView::OnSectionResize(int index, int prev_size, int new_size)
{
  if (!m_is_in_interactive_mode)
  {
    return;
  }

  m_favorite_state = saveState();
}

}  // namespace sup::gui
