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

#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QMenu>

namespace
{

/**
 * @brief Returns an action to trigger "close current project" request.
 * Action's ownership will belong to project handler.
 */
QAction *GetNewProjectAction(sup::gui::ProjectHandler &handler)
{
  auto result = new QAction("&New Project", &handler);
  result->setShortcuts(QKeySequence::New);
  result->setStatusTip("Create a new project");
  QObject::connect(result, &QAction::triggered, &handler,
                   &sup::gui::ProjectHandler::CreateNewProject);
  return result;
}

/**
 * @brief Returns an action to trigger "open existing project" request.
 * Action's ownership will belong to project handler.
 */
QAction *GetOpenExistingProjectAction(sup::gui::ProjectHandler &handler)
{
  auto result = new QAction("&Open Project", &handler);
  result->setShortcuts(QKeySequence::Open);
  result->setStatusTip("Open an existing project");
  QObject::connect(result, &QAction::triggered, &handler,
                   [&handler]() { handler.OpenExistingProject({}); });
  return result;
}

/**
 * @brief Returns an action to trigger "save current project" request.
 * Action's ownership will belong to project handler.
 */
QAction *GetSaveCurrentProjectAction(sup::gui::ProjectHandler &handler)
{
  auto result = new QAction("&Save Project", &handler);
  result->setShortcuts(QKeySequence::Save);
  result->setStatusTip("Save project");
  result->setShortcutContext(Qt::ApplicationShortcut);
  QObject::connect(result, &QAction::triggered, &handler,
                   &sup::gui::ProjectHandler::SaveCurrentProject);
  return result;
}

/**
 * @brief Returns an action to trigger "save project as" request.
 * Action's ownership will belong to project handler.
 */
QAction *GetSaveProjectAsAction(sup::gui::ProjectHandler &handler)
{
  auto result = new QAction("Save &As...", &handler);
  result->setShortcuts(QKeySequence::SaveAs);
  result->setStatusTip("Save project under different name");
  QObject::connect(result, &QAction::triggered, &handler, &sup::gui::ProjectHandler::SaveProjectAs);
  return result;
}

}  // namespace

namespace sup::gui
{

void AddNewProjectAction(QMenu *menu, ProjectHandler &handler)
{
  menu->addAction(GetNewProjectAction(handler));
}

void AddOpenExistingProjectAction(QMenu *menu, ProjectHandler &handler)
{
  menu->addAction(GetOpenExistingProjectAction(handler));
}

void AddSaveCurrentProjectAction(QMenu *menu, ProjectHandler &handler)
{
  menu->addAction(GetSaveCurrentProjectAction(handler));
}

void AddSaveProjectAsAction(QMenu *menu, ProjectHandler &handler)
{
  menu->addAction(GetSaveProjectAsAction(handler));
}

void AddRecentProjectActions(QMenu *menu, ProjectHandler &handler)
{
  auto recent_projects = handler.GetRecentProjectList();
  menu->clear();
  menu->setEnabled(!recent_projects.isEmpty());

  for (const auto &project_dir : recent_projects)
  {
    auto trimmed_project_dir = mvvm::utils::WithTildeHomePath(project_dir);
    auto action = menu->addAction(trimmed_project_dir);
    action->setData(QVariant::fromValue(project_dir));
    auto on_project_selected = [&handler, project_dir]()
    { handler.OpenExistingProject(project_dir); };
    QObject::connect(action, &QAction::triggered, on_project_selected);
  }

  if (!recent_projects.empty())
  {
    menu->addSeparator();
    auto action = menu->addAction("Clear Menu");
    QAction::connect(action, &QAction::triggered, &handler,
                     &sup::gui::ProjectHandler::ClearRecentProjectsList);
  }
}

}  // namespace sup::gui
