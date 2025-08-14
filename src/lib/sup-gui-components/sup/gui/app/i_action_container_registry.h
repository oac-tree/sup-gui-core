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

#include <QString>

class QAction;

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
   * @brief RegisterAction
   * @param container_name
   * @param action
   * @return
   */
  virtual bool RegisterAction(const QString& container_name, QAction* action) = 0;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_I_ACTION_CONTAINER_REGISTRY_H_
