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

#ifndef SUP_GUI_WIDGETS_ACTION_MENU_H_
#define SUP_GUI_WIDGETS_ACTION_MENU_H_

#include <QWidgetAction>

namespace sup::gui
{

/**
 * @brief The ActionMenu class provides a menu with other actions intended for both taskbars and
 * pop-up menus.
 *
 * When inserted in a pop-up menu, it looks like a usual action with the menu. When inserted in a
 * toolbar, it looks like a tool button with an instant menu pop-up feature.
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
