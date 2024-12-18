/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Rafael Silva (EXT)
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

#ifndef SUP_GUI_STYLE_COLORED_ICON_ENGINE_H_
#define SUP_GUI_STYLE_COLORED_ICON_ENGINE_H_

#include <QColor>
#include <QHash>
#include <QIcon>
#include <QIconEngine>
#include <QPalette>

namespace sup::gui
{

/**
 * @brief The ColoredIconEngine class is intended to render monochrome svg-based icons using
 * user-provided colors.
 */
class ColoredIconEngine : public QIconEngine
{
public:
  ColoredIconEngine(const QIcon& icon, const QColor& color_on, const QColor& color_off,
                    const QColor& color_disabled);
  ColoredIconEngine(const QIcon& icon, const QColor& color_on, const QColor& color_off);
  ColoredIconEngine(const QIcon& icon, const QColor& icon_color);
  ColoredIconEngine(const QIcon& icon);
  ColoredIconEngine(const ColoredIconEngine& other);
  ~ColoredIconEngine() override = default;

  void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) override;

  QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override;

  QSize actualSize(const QSize& size, QIcon::Mode mode, QIcon::State state) override;

  QString key() const override;

  QIconEngine* clone() const override;

private:
  void AddMapping(const QColor& color, QIcon::Mode mode, QIcon::State state);
  void AddMappings(const QColor& color, const QList<QIcon::Mode>& modes,
                   const QList<QIcon::State>& states = {QIcon::On, QIcon::Off});

  QHash<QPair<QIcon::Mode, QIcon::State>, QColor> m_color_map;
  QIcon m_icon;
};

}  // namespace sup::gui

#endif  // SUP_GUI_STYLE_COLORED_ICON_ENGINE_H_
