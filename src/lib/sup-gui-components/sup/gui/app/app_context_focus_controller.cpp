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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "app_context_focus_controller.h"

#include "app_command_manager.h"
#include "app_context.h"
#include "app_context_manager.h"

#include <QApplication>
#include <QDebug>
#include <QMenuBar>

namespace sup::gui
{

namespace
{
const bool kPrintDebugMessages = false;
}

AppContextFocusController::AppContextFocusController(AppContextManager &context_manager,
                                                     AppCommandManager &command_manager,
                                                     QObject *parent)
    : QObject(parent), m_context_manager(context_manager), m_command_manager(command_manager)
{
}

AppContextFocusController::~AppContextFocusController() = default;

void AppContextFocusController::OnFocusWidgetUpdate(QWidget *old, QWidget *now)
{
  Q_UNUSED(old)

  // Prevent changing the context object just because the menu or a menu item is activated
  if (qobject_cast<QMenuBar *>(now) || qobject_cast<QMenu *>(now))
  {
    return;
  }

  std::vector<sup::gui::AppContext> context_summary;

  if (kPrintDebugMessages)
  {
    qDebug() << "==============================================================================";
  }

  auto current = now;
  while (current)
  {
    if (kPrintDebugMessages)
    {
      qDebug() << current->metaObject()->className();
    }

    if (m_context_manager.HasContext(current))
    {
      context_summary.push_back(m_context_manager.GetContext(current));
    }
    current = current->parentWidget();
  }

  m_command_manager.SetContextStack(context_summary);

  if (kPrintDebugMessages)
  {
    for (const auto &context : context_summary)
    {
      qDebug() << " context:" << context.GetContextId()
               << m_context_manager.GetWidget(context)->metaObject()->className();
    }
  }
}

}  // namespace sup::gui
