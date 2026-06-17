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

#include "scroll_area_mirror_controller.h"

#include <QAbstractScrollArea>
#include <QScrollBar>
#include <algorithm>

namespace sup::gui
{

ScrollAreaMirrorController::ScrollAreaMirrorController(QObject* parent_object)
    : QObject(parent_object)
{
}

ScrollAreaMirrorController::~ScrollAreaMirrorController() = default;

void ScrollAreaMirrorController::AddScrollArea(QAbstractScrollArea* area)
{
  if (!area || std::find(m_areas.begin(), m_areas.end(), area) != m_areas.end())
  {
    return;
  }

  m_areas.push_back(area);

  connect(area->horizontalScrollBar(), &QAbstractSlider::valueChanged, this,
          [this, area](int value)
          { OnScrollBarValueChanged(area, Qt::Horizontal, value); });

  connect(area->verticalScrollBar(), &QAbstractSlider::valueChanged, this,
          [this, area](int value)
          { OnScrollBarValueChanged(area, Qt::Vertical, value); });

  // Remove from list automatically when the area is destroyed.
  connect(area, &QObject::destroyed, this,
          [this, area]()
          {
            m_areas.erase(std::remove(m_areas.begin(), m_areas.end(), area), m_areas.end());
          });
}

void ScrollAreaMirrorController::RemoveScrollArea(QAbstractScrollArea* area)
{
  auto it = std::find(m_areas.begin(), m_areas.end(), area);
  if (it == m_areas.end())
  {
    return;
  }

  m_areas.erase(it);

  disconnect(area->horizontalScrollBar(), nullptr, this, nullptr);
  disconnect(area->verticalScrollBar(), nullptr, this, nullptr);
  disconnect(area, nullptr, this, nullptr);
}

void ScrollAreaMirrorController::SetActive(bool value)
{
  m_is_active = value;
}

bool ScrollAreaMirrorController::IsActive() const
{
  return m_is_active;
}

void ScrollAreaMirrorController::OnScrollBarValueChanged(QAbstractScrollArea* source,
                                                         Qt::Orientation orientation, int value)
{
  if (!m_is_active || m_is_propagating)
  {
    return;
  }

  m_is_propagating = true;
  for (auto* area : m_areas)
  {
    if (area != source)
    {
      auto* bar =
          (orientation == Qt::Horizontal) ? area->horizontalScrollBar() : area->verticalScrollBar();
      bar->setValue(value);
    }
  }
  m_is_propagating = false;
}

}  // namespace sup::gui
