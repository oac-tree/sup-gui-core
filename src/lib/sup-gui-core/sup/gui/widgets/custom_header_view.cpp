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

#include <QDebug>

namespace sup::gui
{

CustomHeaderView::CustomHeaderView(QWidget *parent) : QHeaderView(Qt::Horizontal, parent)
{
  connect(this, &QHeaderView::sectionResized, this, &CustomHeaderView::OnSectionResize);
}

void CustomHeaderView::RestoreFavoriteState()
{
  //  for (size_t i = 0; i < m_section_size.size(); ++i)
  //  {
  //    qDebug() << "RestoreSize" << (this) << i << m_section_size[i];
  //    resizeSection(i, m_section_size[i]);
  //  }
  qDebug() << "RestoreSize";
  restoreState(m_favorite_state);
}

bool CustomHeaderView::IsAdjustedByUser() const
{
  return m_is_adjusted_by_user;
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

void CustomHeaderView::OnSectionResize(int index, int prev_size, int new_size)
{
  qDebug() << "OnSectionResize 2.1" << (this);
  for (int i = 0; i < this->count(); ++i)
  {
    qDebug() << "    " << i << sectionSize(i);
  }

  if (!m_is_in_interactive_mode)
  {
    return;
  }

  qDebug() << "OnSectionResize 2.1";

  m_is_adjusted_by_user = true;

  m_favorite_state = saveState();

//  m_section_size.resize(this->count());

//  for (int i = 0; i < this->count(); ++i)
//  {
//    m_section_size[i] = sectionSize(i);
//  }
}

}  // namespace sup::gui
