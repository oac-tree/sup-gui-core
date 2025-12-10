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

#include "app_context_focus_controller.h"

#include "app_command_context.h"
#include "i_app_command_context_widget_map.h"
#include "i_app_command_registry.h"

#include <QApplication>
#include <QDebug>
#include <QMenuBar>

namespace sup::gui
{

AppContextFocusController::AppContextFocusController(IAppCommandContextWidgetMap& widget_map,
                                                     IAppCommandRegistry& command_registry)
    : m_widget_map(widget_map), m_command_registry(command_registry)
{
}

AppContextFocusController::~AppContextFocusController() = default;

void AppContextFocusController::OnFocusWidgetUpdate(const QWidget *old, QWidget* now)
{
  Q_UNUSED(old)

  // Prevent changing the context object just because the menu or a menu item is activated
  if ((qobject_cast<QMenuBar*>(now) != nullptr) || (qobject_cast<QMenu*>(now) != nullptr))
  {
    return;
  }

  std::vector<sup::gui::AppCommandContext> context_summary;

  auto current = now;
  while (current != nullptr)
  {
    if (m_widget_map.HasContext(current))
    {
      context_summary.push_back(m_widget_map.GetContext(current));
    }
    current = current->parentWidget();
  }

  m_command_registry.SetContextStack(context_summary);
}

}  // namespace sup::gui
