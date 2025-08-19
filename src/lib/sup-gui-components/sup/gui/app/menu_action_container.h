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

#ifndef SUP_GUI_APP_MENU_ACTION_CONTAINER_H_
#define SUP_GUI_APP_MENU_ACTION_CONTAINER_H_

#include <sup/gui/app/i_action_container.h>

#include <QString>
#include <vector>

class QAction;
class QMenuBar;
class QMenu;

namespace sup::gui
{

/**
 * @brief The MenuActionContainer class holds information about a menu and actions added to it.
 */
class MenuActionContainer : public IActionContainer
{
public:
  /**
   * @brief Main constructor.
   *
   * @param name The name of the container.
   * @param menu The menu to serve.
   */
  MenuActionContainer(const QString& name, QMenu* menu);

  QString GetName();

  QMenu* GetMenu() override;

  bool AddAction(QAction* action) override;

  std::size_t GetActionCount() override;

private:
  QString m_name;
  QMenu* m_menu{nullptr};
  std::vector<QAction*> m_actions;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_MENU_ACTION_CONTAINER_H_
