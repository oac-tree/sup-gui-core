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

#ifndef SUP_GUI_MAINWINDOW_SESSION_ITEM_WIDGET_H_
#define SUP_GUI_MAINWINDOW_SESSION_ITEM_WIDGET_H_

#include <QWidget>

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{

/**
 * @brief The SessionItemWidget class is a base for all widgets which can show one item at a time.
 */
class SessionItemWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SessionItemWidget(QWidget* parent_widget = nullptr);

  virtual void SetItem(mvvm::SessionItem* item);
};

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_SETTINGS_EDITOR_H_
