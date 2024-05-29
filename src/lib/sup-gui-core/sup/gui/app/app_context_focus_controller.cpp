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

#include "app_context_focus_controller.h"

#include "app_command_manager.h"

#include <QApplication>
#include <QMenuBar>

namespace sup::gui
{

AppContextFocusController::AppContextFocusController(AppCommandManager &command_manager,
                                                                 QObject *parent)
    : QObject(parent), m_command_manager(command_manager)
{
}

AppContextFocusController::~AppContextFocusController() = default;

void AppContextFocusController::UpdateFocusWidget(QWidget *old, QWidget *now)
{
  Q_UNUSED(old)

  // Prevent changing the context object just because the menu or a menu item is activated
  if (qobject_cast<QMenuBar *>(now) || qobject_cast<QMenu *>(now))
  {
    return;
  }

  // TODO implement processing focus widget into context
}

}  // namespace sup::gui
