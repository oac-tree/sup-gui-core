/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "overlay_widget_position_strategy.h"

#include <mvvm/widgets/widget_utils.h>

#include <QAbstractScrollArea>
#include <QEvent>
#include <QScrollBar>

namespace
{

/**
 * @brief Returns horizontal distance to the widget corner.
 */
int GetXOffset()
{
  return mvvm::utils::UnitSize();
}

/**
 * @brief Returns vertical distance to the widget corner.
 */
int GetYOffset()
{
  return mvvm::utils::UnitSize();
}

}  // namespace

namespace sup::gui
{

OverlayWidgetPositionStrategy::OverlayWidgetPositionStrategy(QWidget* overlay_widget, QWidget* area,
                                                             QObject* parent_object)
    : QObject(parent_object), m_overlay_widget(overlay_widget), m_area(area)
{
  m_area->installEventFilter(this);
  m_overlay_widget->show();
  UpdateOverlayWidgetGeometry();
}

bool OverlayWidgetPositionStrategy::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::Resize)
  {
    UpdateOverlayWidgetGeometry();
  }

  return QObject::eventFilter(obj, event);
}

void OverlayWidgetPositionStrategy::UpdateOverlayWidgetGeometry()
{
  auto pos = CalculateWidgetPosition();
  m_overlay_widget->setGeometry(pos.x(), pos.y(), m_overlay_widget->width(),
                                m_overlay_widget->height());
}

QPoint OverlayWidgetPositionStrategy::CalculateWidgetPosition() const
{
  int x = m_area->width() - m_overlay_widget->width() - GetXOffset();
  int y = m_area->height() - m_overlay_widget->height() - GetYOffset();

  // shift position a bit, if scroll bars are present and visible
  if (auto scroll_area = dynamic_cast<QAbstractScrollArea*>(m_area); scroll_area)
  {
    if (QScrollBar* horizontal = scroll_area->horizontalScrollBar();
        horizontal && horizontal->isVisible())
    {
      y -= horizontal->height();
    }

    if (QScrollBar* vertical = scroll_area->verticalScrollBar(); vertical && vertical->isVisible())
    {
      x -= vertical->width();
    }
  }

  return {x, y};
}

}  // namespace sup::gui
