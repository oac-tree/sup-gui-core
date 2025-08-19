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

#ifndef SUP_GUI_APP_I_ACTION_CONTAINER_REGISTRY_H_
#define SUP_GUI_APP_I_ACTION_CONTAINER_REGISTRY_H_

#include <sup/gui/app/i_action_container.h>

#include <QString>

class QAction;
class QMenuBar;

namespace sup::gui
{

/**
 * @brief The IActionContainerRegistry is an interface that handles the collection of action
 * containers.
 *
 * An action container is normally associated with a flat menu, while a collection of action
 * containers can be associated with the menu bar.
 */
class IActionContainerRegistry
{
public:
  IActionContainerRegistry() = default;
  virtual ~IActionContainerRegistry() = default;

  IActionContainerRegistry(const IActionContainerRegistry&) = delete;
  IActionContainerRegistry& operator=(const IActionContainerRegistry&) = delete;
  IActionContainerRegistry(IActionContainerRegistry&&) = delete;
  IActionContainerRegistry& operator=(IActionContainerRegistry&&) = delete;

  /**
   * @brief Sets menubar to serve.
   *
   * There can be only one menubar to serve.
   */
  virtual void SetMenuBar(QMenuBar* menubar) = 0;

  /**
   * @brief Adds new menu to the menu bar.
   *
   * The menubar will take an ownership. If menu was already created, just returns action container.
   *
   * @param container_name The title of the menu.
   * @param menubar The menubar where the menu will be added.
   *
   * @return Action container holding just created menu and other information.
   */
  virtual IActionContainer* AddMenu(const QString& container_name) = 0;

  /**
   * @brief Returns action container with the given name.
   */
  virtual IActionContainer* GetContainer(const QString& container_name) = 0;

  /**
   * @brief Registers action and add it to the menu.
   *
   * The ownership of the action is not taken.
   *
   * @param menu_name The name of already existing menu added via AddMenu call.
   * @param action User action to add.
   * @return True if action was added, or false if no such menu was registered.
   */
  virtual bool RegisterAction(const QString& container_name, QAction* action) = 0;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_I_ACTION_CONTAINER_REGISTRY_H_
