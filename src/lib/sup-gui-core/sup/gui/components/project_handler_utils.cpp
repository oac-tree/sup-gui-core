/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "project_handler_utils.h"

#include "project_handler.h"

#include <QAction>
#include <QMenu>

namespace sup::gui
{

QAction *GetNewProjectAction(ProjectHandler &handler)
{
  auto result = new QAction("&New Project", &handler);
  result->setShortcuts(QKeySequence::New);
  result->setStatusTip("Create a new project");
  QObject::connect(result, &QAction::triggered, &handler, &ProjectHandler::CreateNewProject);
  return result;
}

void AddNewProjectAction(QMenu *menu, ProjectHandler &handler)
{
  menu->addAction(GetNewProjectAction(handler));
}

QAction *GetOpenExistingProjectAction(ProjectHandler &handler)
{
  auto result = new QAction("&Open Project", &handler);
  result->setShortcuts(QKeySequence::Open);
  result->setStatusTip("Open an existing project");
  QObject::connect(result, &QAction::triggered, &handler,
                   [&handler]() { handler.OpenExistingProject({}); });
  return result;
}

void AddOpenExistingProjectAction(QMenu *menu, ProjectHandler &handler)
{
  menu->addAction(GetOpenExistingProjectAction(handler));
}

QAction *GetSaveCurrentProjectAction(ProjectHandler &handler)
{
  auto result = new QAction("&Save Project", &handler);
  result->setShortcuts(QKeySequence::Save);
  result->setStatusTip("Save project");
  result->setShortcutContext(Qt::ApplicationShortcut);
  QObject::connect(result, &QAction::triggered, &handler,
                   &sup::gui::ProjectHandler::SaveCurrentProject);
  return result;
}

void AddSaveCurrentProjectAction(QMenu *menu, ProjectHandler &handler)
{
  menu->addAction(GetSaveCurrentProjectAction(handler));
}

QAction *GetSaveProjectAsAction(ProjectHandler &handler)
{
  auto result = new QAction("Save &As...", &handler);
  result->setShortcuts(QKeySequence::SaveAs);
  result->setStatusTip("Save project under different name");
  QObject::connect(result, &QAction::triggered, &handler, &sup::gui::ProjectHandler::SaveProjectAs);
  return result;
}

void AddSaveProjectAsAction(QMenu *menu, ProjectHandler &handler)
{
  menu->addAction(GetSaveProjectAsAction(handler));
}

}  // namespace sup::gui
