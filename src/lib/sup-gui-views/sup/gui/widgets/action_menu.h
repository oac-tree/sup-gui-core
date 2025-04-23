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

#ifndef SUP_GUI_WIDGETS_ACTION_MENU_H_
#define SUP_GUI_WIDGETS_ACTION_MENU_H_

#include <QWidgetAction>

namespace sup::gui
{

/**
 * @brief The ActionMenu class is an action with the menu and instant pop-up capabilities.
 *
 * When inserted in a context menu (i.e. right-mouse-button menu), it looks like a usual action with
 * the menu. When inserted in a toolbar, it looks like a tool button with an instant menu pop-up
 * feature.
 *
 * It is necessary since normal actions with the menu doesn't have instant pop-up when embedded in a
 * toolbar.
 *
 * Inspired by https://api.kde.org/frameworks/kwidgetsaddons/html/classKActionMenu.html
 */
class ActionMenu : public QWidgetAction
{
  Q_OBJECT

public:
  explicit ActionMenu(QObject *parent = nullptr);
  ActionMenu(const QString &text, QObject *parent);
  ActionMenu(const QIcon &icon, const QString &text, QObject *parent);

protected:
  QWidget *createWidget(QWidget *parent) override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_ACTION_MENU_H_
