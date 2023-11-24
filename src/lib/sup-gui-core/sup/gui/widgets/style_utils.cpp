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

#include "style_utils.h"

#include <mvvm/widgets/widget_utils.h>

#include <QApplication>
#include <QIcon>
#include <QSize>
#include <QTreeView>
#include <cmath>

namespace
{
const QString DefaultIconExtension("svg");

/**
 * @brief Generate style string for the tree with vertical lines connecting parent and children.
 */
QString CreatePopertyTreeStyleString()
{
  QString result = R"(
  QTreeView::branch:has-siblings:!adjoins-item {
      border-image: url(:/icons/vline.png) 0;
  }

  QTreeView::branch:has-siblings:adjoins-item {
      border-image: url(:/icons/branch-more.png) 0;
  }

  QTreeView::branch:!has-children:!has-siblings:adjoins-item {
      border-image: url(:/icons/branch-end.png) 0;
  }

  QTreeView::branch:has-children:!has-siblings:closed,
  QTreeView::branch:closed:has-children:has-siblings {
          padding:2px 2px 2px 2px;border-image: none;
          image: url(:/icons/chevron-right.png);
  }

  QTreeView::branch:open:has-children:!has-siblings,
  QTreeView::branch:open:has-children:has-siblings  {
          padding:2px 2px 2px 2px;border-image: none;
          image: url(:/icons/chevron-down.png);
  })";

  return result;
}

}  // namespace

namespace sup::gui::utils
{

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

QIcon GetIcon(const QString &icon_name)
{
  const QString resource_name = icon_name.contains(".")
                                    ? QString(":/icons/%1").arg(icon_name)
                                    : QString(":/icons/%1.%2").arg(icon_name, DefaultIconExtension);
  return QIcon(resource_name);
}

void BeautifyTreeStyle(QTreeView *tree)
{
  if (QApplication::style()->objectName() == QString("fusion"))
  {
    tree->setStyleSheet(CreatePopertyTreeStyleString());
  }
}

}  // namespace sup::gui::utils
