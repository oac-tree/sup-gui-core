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

#ifndef SUP_GUI_WIDGETS_STEADY_MENU_H_
#define SUP_GUI_WIDGETS_STEADY_MENU_H_

#include <QMenu>

namespace sup::gui
{

/**
 * @brief The SteadyMenu class is a menu that doesn't close after the user has made its choice.
 *
 * It is intended to show checkable actions and prevent the menu from closing while the user
 * checks/uncheck actions in the list.
 */
class SteadyMenu : public QMenu
{
  Q_OBJECT

public:
  explicit SteadyMenu(QWidget* parent_widget = nullptr);

protected:
  void mouseReleaseEvent(QMouseEvent* event) override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_STEADY_MENU_H_
