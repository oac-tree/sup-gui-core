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

#include "default_command_service.h"

#include "app_command_context_widget_map.h"
#include "app_command_registry.h"
#include "app_command_service.h"
#include "menu_bar_action_registry.h"

#include <QMainWindow>

namespace sup::gui
{

std::unique_ptr<IAppCommandService> CreateDefaultCommandService()
{
  auto command_registry = std::make_unique<AppCommandRegistry>();
  auto context_widget_map = std::make_unique<AppCommandContextWidgetMap>();

  auto menubar_registry = std::make_unique<MenuBarActionRegistry>();
  return std::make_unique<AppCommandService>(
      std::move(command_registry), std::move(context_widget_map), std::move(menubar_registry));
}

}  // namespace sup::gui
