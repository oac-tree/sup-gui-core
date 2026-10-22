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

#include "app_context_focus_controller.h"

#include "app_command_context.h"
#include "i_app_command_context_widget_map.h"
#include "i_app_command_registry.h"

#include <QApplication>
#include <QDebug>
#include <QMenuBar>

namespace sup::gui
{

namespace
{

//! Returns contexts of the widget and its parents, innermost first.
std::vector<AppCommandContext> GetContextStack(const IAppCommandContextWidgetMap& widget_map,
                                               const QWidget* widget)
{
  std::vector<AppCommandContext> result;
  for (auto current = widget; current != nullptr; current = current->parentWidget())
  {
    if (widget_map.HasContext(current))
    {
      result.push_back(widget_map.GetContext(current));
    }
  }
  return result;
}

}  // namespace

AppContextFocusController::AppContextFocusController(IAppCommandContextWidgetMap& widget_map,
                                                     IAppCommandRegistry& command_registry)
    : m_widget_map(widget_map), m_command_registry(command_registry)
{
}

void AppContextFocusController::OnFocusWidgetUpdate(const QWidget* old, QWidget* now)
{
  Q_UNUSED(old)

  // Prevent changing the context object just because the menu or a menu item is activated
  if ((qobject_cast<QMenuBar*>(now) != nullptr) || (qobject_cast<QMenu*>(now) != nullptr))
  {
    return;
  }

  m_focus_widget = now;

  if (m_is_updating)
  {
    // nested call during the command update, the running update applies the new widget
    return;
  }

  m_is_updating = true;
  QPointer<QWidget> applied_widget;
  do
  {
    applied_widget = m_focus_widget;
    m_command_registry.SetContextStack(GetContextStack(m_widget_map, applied_widget));
  } while (applied_widget != m_focus_widget);
  m_is_updating = false;
}

}  // namespace sup::gui
