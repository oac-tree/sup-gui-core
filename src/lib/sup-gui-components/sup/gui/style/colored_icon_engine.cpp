/******************************************************************************
 *
 * Project       : PSPS editor
 *
 * Description   : Graphical user interface for pulse schedule preparation system
 *
 * Author        : Rafael Silva (EXT)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "colored_icon_engine.h"

#include <QApplication>
#include <QIcon>
#include <QPainter>
#include <QPixmap>

namespace sup::gui
{

ColoredIconEngine::ColoredIconEngine(const QIcon& icon, const QColor& color_on,
                                     const QColor& color_off, const QColor& color_disabled)
    : m_icon{icon}
{
  AddMappings(color_disabled, {QIcon::Disabled});

  AddMappings(color_on, {QIcon::Normal, QIcon::Active}, {QIcon::On});
  AddMappings(color_off, {QIcon::Normal, QIcon::Active}, {QIcon::Off});

  // Make selected icon 20% lighter when selected
  AddMapping(color_on.lighter(120), QIcon::Selected, QIcon::On);
  AddMapping(color_off.lighter(120), QIcon::Selected, QIcon::Off);
}

ColoredIconEngine::ColoredIconEngine(const QIcon& icon, const QColor& color_on,
                                     const QColor& color_off)
    : ColoredIconEngine(icon, color_on, color_off,
                        QApplication::palette().color(QPalette::Disabled, QPalette::WindowText))
{
}

ColoredIconEngine::ColoredIconEngine(const QIcon& icon, const QColor& icon_color)
    : ColoredIconEngine(icon, icon_color, icon_color)
{
}

ColoredIconEngine::ColoredIconEngine(const QIcon& icon)
    : ColoredIconEngine(icon, QApplication::palette().color(QPalette::Active, QPalette::WindowText))
{
}

ColoredIconEngine::ColoredIconEngine(const ColoredIconEngine& other)
    : QIconEngine(other), m_icon{other.m_icon}, m_color_map{other.m_color_map}
{
}

void ColoredIconEngine::paint(QPainter* painter, const QRect& rect, QIcon::Mode mode,
                              QIcon::State state)
{
  if (m_icon.isNull())
  {
    painter->setCompositionMode(QPainter::CompositionMode_Clear);
    painter->eraseRect(rect);
    return;
  }

  painter->setCompositionMode(QPainter::CompositionMode_Source);
  painter->drawPixmap(rect, pixmap(rect.size(), mode, state));
}

QPixmap ColoredIconEngine::pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state)
{
  // TODO: Implement QPixmap caching to mitigate the performance impact

  if (m_icon.isNull())
  {
    return QPixmap();
  }

  QPixmap pix{size};
  pix.fill(Qt::transparent);

  QRect rect{{}, size};
  QPainter painter(&pix);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  painter.drawPixmap(rect, m_icon.pixmap(size, mode, state));
  painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
  painter.fillRect(rect, m_color_map.value({mode, state}));

  return pix;
}

QSize ColoredIconEngine::actualSize(const QSize& size, QIcon::Mode mode, QIcon::State state)
{
  return m_icon.actualSize(size, mode, state);
}

QString ColoredIconEngine::key() const
{
  return "ColoredIconEngine";
}

QIconEngine* ColoredIconEngine::clone() const
{
  return new ColoredIconEngine(*this);
}

void ColoredIconEngine::AddMapping(const QColor& color, QIcon::Mode mode, QIcon::State state)
{
  (void) m_color_map.insert({mode, state}, color);
}

void ColoredIconEngine::AddMappings(const QColor& color, const QList<QIcon::Mode>& modes,
                                    const QList<QIcon::State>& states)
{
  for (const auto& mode : modes)
  {
    for (const auto& state : states)
    {
      AddMapping(color, mode, state);
    }
  }
}

}  // namespace sup::gui
