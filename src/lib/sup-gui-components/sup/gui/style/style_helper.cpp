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

#include "style_helper.h"

#include "colored_icon_engine.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/widgets/widget_utils.h>

#include <QApplication>
#include <QDir>
#include <QIcon>
#include <QResource>
#include <QSize>
#include <QTreeView>
#include <cmath>

namespace sup::gui::utils
{

namespace
{

const bool kUseColorEngine = true;

const QString kLightIconColor("#e6e6e6");
const QString kDarkIconColor("#4d4d4d");

/**
 * @brief Generate style string for the tree with vertical lines connecting parent and children.
 */
QString CreatePopertyTreeStyleString()
{
  QString result = R"(
  QTreeView::branch:has-siblings:!adjoins-item {
      border-image: url(:/icons/vline.svg) 0;
  }

  QTreeView::branch:has-siblings:adjoins-item {
      border-image: url(:/icons/branch-more.svg) 0;
  }

  QTreeView::branch:!has-children:!has-siblings:adjoins-item {
      border-image: url(:/icons/branch-end.svg) 0;
  }

  QTreeView::branch:has-children:!has-siblings:closed,
  QTreeView::branch:closed:has-children:has-siblings {
          padding:2px 2px 2px 2px;border-image: none;
          image: url(:/icons/chevron-right.svg);
  }

  QTreeView::branch:open:has-children:!has-siblings,
  QTreeView::branch:open:has-children:has-siblings  {
          padding:2px 2px 2px 2px;border-image: none;
          image: url(:/icons/chevron-down.svg);
  })";

  return result;
}

/**
 * @brief Creates color engine necessary to render given icon.
 */
std::unique_ptr<QIconEngine> CreateColorEngine(const QIcon &icon, IconColorFlavor icon_flavor)
{
  return std::make_unique<ColoredIconEngine>(icon, GetIconBaseColor(icon_flavor));
}

/**
 * @brief Creates icon colored in given flavor.
 */
QIcon CreateColoredIcon(const QIcon &icon, IconColorFlavor icon_flavor)
{
  return QIcon(CreateColorEngine(icon, icon_flavor).release());  // icon takes ownership over engine
}

}  // namespace

bool IsDarkTheme()
{
  if (QApplication::instance())
  {
    const auto palette = QApplication::palette();
    return palette.color(QPalette::WindowText).lightness()
           > palette.color(QPalette::Window).lightness();
  }

  return false;
}

QColor GetIconBaseColor(IconColorFlavor icon_flavor)
{
  // if icon flavor is specified, return fixed flavor's color
  if (icon_flavor == IconColorFlavor::kForDarkThemes)
  {
    return kLightIconColor;
  }

  if (icon_flavor == IconColorFlavor::kForLightThemes)
  {
    return kDarkIconColor;
  }

  // if flavor is unspecified, return color depending on theme itself
  if (icon_flavor == IconColorFlavor::kUnspecified)
  {
    return IsDarkTheme() ? kLightIconColor : kDarkIconColor;
  }

  throw mvvm::RuntimeException("Unknown icon flavor");
}

QSize ToolBarIconSize()
{
  const int width = mvvm::utils::UnitSize(2.3);
  return {width, width};
}

QSize NarrowToolBarIconSize()
{
  const int width = std::round(mvvm::utils::UnitSize(1.75));
  return {width, width};
}

QIcon GetIcon(const QString &resource_name, IconColorFlavor icon_flavor)
{
  QIcon icon(resource_name);
  return kUseColorEngine ? CreateColoredIcon(icon, icon_flavor) : icon;
}

QIcon FindIcon(const QString &icon_name, IconColorFlavor icon_flavor)
{
  // in accordance with the sup_ugi_core_icons.qrc file
  return GetIcon(QString(":/sup-gui-core/icons/%1.svg").arg(icon_name), icon_flavor);
}

void BeautifyTreeStyle(QTreeView *tree)
{
  if (QApplication::style()->objectName() == QString("fusion"))
  {
    tree->setStyleSheet(CreatePopertyTreeStyleString());
  }
}

}  // namespace sup::gui::utils
