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

#include "flow_layout.h"

#include <QtWidgets>

namespace sup::gui
{

FlowLayout::FlowLayout(QWidget* parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
  setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::FlowLayout(int margin, int hSpacing, int vSpacing)
    : m_hSpace(hSpacing), m_vSpace(vSpacing)
{
  setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout()
{
  const QLayoutItem* item = takeAt(0);
  while (item != nullptr)
  {
    delete item;
    item = takeAt(0);
  }
}

void FlowLayout::addItem(QLayoutItem* item)
{
  // ownership of item is transferred to the layout
  m_item_list.append(item);
}

int FlowLayout::horizontalSpacing() const
{
  if (m_hSpace >= 0)
  {
    return m_hSpace;
  }
  return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int FlowLayout::verticalSpacing() const
{
  if (m_vSpace >= 0)
  {
    return m_vSpace;
  }
  return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

int FlowLayout::count() const
{
  return static_cast<int>(m_item_list.size());
}

QLayoutItem* FlowLayout::itemAt(int index) const
{
  return m_item_list.value(index);
}

QLayoutItem* FlowLayout::takeAt(int index)
{
  if (index >= 0 && index < m_item_list.size())
  {
    return m_item_list.takeAt(index);
  }

  return nullptr;
}

Qt::Orientations FlowLayout::expandingDirections() const
{
  return {};
}

bool FlowLayout::hasHeightForWidth() const
{
  return true;
}

int FlowLayout::heightForWidth(int width) const
{
  // To adjust to widgets of which height is dependent on width, we implement
  return doLayout(QRect(0, 0, width, 0), true);
}

void FlowLayout::setGeometry(const QRect& rect)
{
  QLayout::setGeometry(rect);
  doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const
{
  return minimumSize();
}

QSize FlowLayout::minimumSize() const
{
  QSize size;
  for (const QLayoutItem* item : std::as_const(m_item_list))
  {
    size = size.expandedTo(item->minimumSize());
  }

  const QMargins margins = contentsMargins();
  size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
  return size;
}

int FlowLayout::doLayout(const QRect& rect, bool test_only) const
{
  // handles the layout if horizontalSpacing() or verticalSpacing() don't return the default value.
  int left{0};
  int top{0};
  int right{0};
  int bottom{0};

  getContentsMargins(&left, &top, &right, &bottom);
  const QRect effective_rect = rect.adjusted(+left, +top, -right, -bottom);
  int x = effective_rect.x();
  int y = effective_rect.y();
  int lineHeight = 0;

  for (QLayoutItem* item : std::as_const(m_item_list))
  {
    const QWidget* wid = item->widget();
    int spaceX = horizontalSpacing();
    if (spaceX == -1)
    {
      spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton,
                                           Qt::Horizontal);
    }
    int spaceY = verticalSpacing();
    if (spaceY == -1)
    {
      spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton,
                                           Qt::Vertical);
    }

    //  sets the proper amount of spacing for each widget in the layout, based on the current style.
    // The position of each item in the layout is then calculated by adding the items width and the
    // line height to the initial x and y coordinates.

    int nextX = x + item->sizeHint().width() + spaceX;
    if (nextX - spaceX > effective_rect.right() && lineHeight > 0)
    {
      x = effective_rect.x();
      y = y + lineHeight + spaceY;
      nextX = x + item->sizeHint().width() + spaceX;
      lineHeight = 0;
    }

    if (!test_only)
    {
      item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
    }

    x = nextX;
    lineHeight = qMax(lineHeight, item->sizeHint().height());
  }
  return y + lineHeight - rect.y() + bottom;
}

int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
  QObject* parent = this->parent();
  if (parent == nullptr)
  {
    return -1;
  }

  if (parent->isWidgetType())
  {
    auto pw = dynamic_cast<QWidget*>(parent);
    return pw->style()->pixelMetric(pm, nullptr, pw);
  }

  return dynamic_cast<QLayout*>(parent)->spacing();
}

}  // namespace sup::gui
