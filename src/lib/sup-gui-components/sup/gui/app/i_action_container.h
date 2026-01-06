/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef SUP_GUI_APP_I_ACTION_CONTAINER_H_
#define SUP_GUI_APP_I_ACTION_CONTAINER_H_

#include <cstddef>

class QMenu;
class QAction;

namespace sup::gui
{

/**
 * @brief The IActionContainer is an interface to store a collection of actions.
 *
 * It is non-owning container, where actions belong to the same group (i.e. flat menu).
 */
class IActionContainer
{
public:
  IActionContainer() = default;
  virtual ~IActionContainer() = default;

  IActionContainer(const IActionContainer&) = delete;
  IActionContainer& operator=(const IActionContainer&) = delete;
  IActionContainer(IActionContainer&&) = delete;
  IActionContainer& operator=(IActionContainer&&) = delete;

  /**
   * @brief Returns number of actions registered in this container.
   */
  virtual std::size_t GetActionCount() = 0;

  /**
   * @brief The menu associated with this container, where actions are added.
   */
  virtual QMenu* GetMenu() = 0;

  /**
   * @brief Adds action to the menu associated with this container.
   *
   * @param The action to add (ownership belongs to the user).
   * @return True if action has been successfully added.
   */
  virtual bool AddAction(QAction* action) = 0;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_I_ACTION_CONTAINER_H_
