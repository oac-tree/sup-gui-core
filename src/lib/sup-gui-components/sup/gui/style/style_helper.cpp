/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
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

#include "style_helper.h"

#include "colored_icon_engine.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/style/mvvm_style_helper.h>

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
  const QString result = R"(
  QTreeView::branch:has-siblings:!adjoins-item {
      border-image: url(:/sup-gui-core/icons/vline.svg) 0;
  }

  QTreeView::branch:has-siblings:adjoins-item {
      border-image: url(:/sup-gui-core/icons/branch-more.svg) 0;
  }

  QTreeView::branch:!has-children:!has-siblings:adjoins-item {
      border-image: url(:/sup-gui-core/icons/branch-end.svg) 0;
  }

  QTreeView::branch:has-children:!has-siblings:closed,
  QTreeView::branch:closed:has-children:has-siblings {
          padding:2px 2px 2px 2px;border-image: none;
          image: url(:/sup-gui-core/icons/chevron-right.svg);
  }

  QTreeView::branch:open:has-children:!has-siblings,
  QTreeView::branch:open:has-children:has-siblings  {
          padding:2px 2px 2px 2px;border-image: none;
          image: url(:/sup-gui-core/icons/chevron-down.svg);
  })";

  return result;
}

/**
 * @brief Creates color engine necessary to render given icon.
 */
std::unique_ptr<QIconEngine> CreateColorEngine(const QIcon &icon, mvvm::ColorFlavor icon_flavor)
{
  return std::make_unique<ColoredIconEngine>(icon, GetIconBaseColor(icon_flavor));
}

/**
 * @brief Creates icon colored in given flavor.
 */
QIcon CreateColoredIcon(const QIcon &icon, mvvm::ColorFlavor icon_flavor)
{
  return QIcon(CreateColorEngine(icon, icon_flavor).release());  // icon takes ownership over engine
}

}  // namespace

QColor GetIconBaseColor(mvvm::ColorFlavor icon_flavor)
{
  // if icon flavor is specified, return fixed flavor's color
  if (icon_flavor == mvvm::ColorFlavor::kForDarkThemes)
  {
    return kLightIconColor;
  }

  if (icon_flavor == mvvm::ColorFlavor::kForLightThemes)
  {
    return kDarkIconColor;
  }

  // if flavor is unspecified, return color depending on theme itself
  if (icon_flavor == mvvm::ColorFlavor::kAuto)
  {
    return mvvm::style::IsDarkTheme() ? kLightIconColor : kDarkIconColor;
  }

  throw mvvm::RuntimeException("Unknown icon flavor");
}

QSize ToolBarIconSize()
{
  const auto width = mvvm::style::UnitSize(2.3);
  return {width, width};
}

QSize NarrowToolBarIconSize()
{
  const auto width = mvvm::style::UnitSize(1.75);
  return {width, width};
}

QIcon GetIcon(const QString &resource_name, mvvm::ColorFlavor icon_flavor)
{
  const QIcon icon(resource_name);
  return kUseColorEngine ? CreateColoredIcon(icon, icon_flavor) : icon;
}

QIcon FindIcon(const QString &icon_name, mvvm::ColorFlavor icon_flavor)
{
  // in accordance with the sup_gui_core_icons.qrc file
  return GetIcon(QString(":/sup-gui-core/icons/%1.svg").arg(icon_name), icon_flavor);
}

void SetCollapseExpandStyle(QTreeView *tree)
{
  tree->setStyleSheet(CreatePopertyTreeStyleString());
}

}  // namespace sup::gui::utils
