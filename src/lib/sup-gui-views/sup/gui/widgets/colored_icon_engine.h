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

#ifndef SUP_GUI_WIDGETS_STYLED_ICON_ENGINE_H_
#define SUP_GUI_WIDGETS_STYLED_ICON_ENGINE_H_

#include <QColor>
#include <QIcon>
#include <QIconEngine>
#include <QPalette>
#include <QHash>

namespace sup::gui
{

//! ColoredIconEngine allows the user to create icons with custom colors by specifying
//! icon_color(_on/_off) and icon_color_disabled, the resulting icon is a monochrome colored shadow
//! of the icon, transparencies are inherited

class ColoredIconEngine : public QIconEngine
{
public:
  ColoredIconEngine(const QIcon& icon, const QColor& icon_color_on, const QColor& icon_color_off,
                    const QColor& icon_color_disabled);
  ColoredIconEngine(const QIcon& icon, const QColor& icon_color_on, const QColor& icon_color_off);
  ColoredIconEngine(const QIcon& icon, const QColor& icon_color);
  ColoredIconEngine(const QIcon& icon);
  ColoredIconEngine(const ColoredIconEngine& other);
  ~ColoredIconEngine() override = default;

  // static void SetUseColoredIcons(bool use_colored_icons);

  void AddMapping(const QColor& color, QIcon::Mode mode, QIcon::State state);
  void AddMappings(const QColor& color, const QList<QIcon::Mode>& modes,
                   const QList<QIcon::State>& states = {QIcon::On, QIcon::Off});

  void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) override;
  QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override;

  QSize actualSize(const QSize& size, QIcon::Mode mode, QIcon::State state) override;

  QString key() const override;
  QIconEngine* clone() const override;

  // void virtual_hook(int id, void* data) override;

private:
  // static bool m_use_colored_icons;
  QHash<QPair<QIcon::Mode, QIcon::State>, QColor> m_color_map;
  QIcon m_icon;
};

}  // namespace pspsconfig

#endif  // SUP_GUI_WIDGETS_STYLED_ICON_ENGINE_H_
