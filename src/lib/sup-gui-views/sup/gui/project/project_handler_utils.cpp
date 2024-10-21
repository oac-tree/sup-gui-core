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

#include "project_handler_utils.h"

#include "i_project_handler.h"

#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QMenu>

namespace
{

/**
 * @brief Creates an action to trigger "create new project" request.
 */
std::unique_ptr<QAction> CreateNewProjectAction(sup::gui::IProjectHandler &handler)
{
  auto result = std::make_unique<QAction>("&New Project");
  result->setShortcuts(QKeySequence::New);
  result->setStatusTip("Create a new project");
  QObject::connect(result.get(), &QAction::triggered, [&handler]() { handler.CreateNewProject(); });
  return result;
}

/**
 * @brief Creates an action to trigger "open existing project" request.
 */
std::unique_ptr<QAction> CreateOpenExistingProjectAction(sup::gui::IProjectHandler &handler)
{
  auto result = std::make_unique<QAction>("&Open Project");
  result->setShortcuts(QKeySequence::Open);
  result->setStatusTip("Open an existing project");
  QObject::connect(result.get(), &QAction::triggered,
                   [&handler]() { handler.OpenExistingProject({}); });
  return result;
}

/**
 * @brief Creates an action to trigger "save current project" request.
 */
std::unique_ptr<QAction> CreateSaveCurrentProjectAction(sup::gui::IProjectHandler &handler)
{
  auto result = std::make_unique<QAction>("&Save Project");
  result->setShortcuts(QKeySequence::Save);
  result->setStatusTip("Save project");
  result->setShortcutContext(Qt::ApplicationShortcut);
  QObject::connect(result.get(), &QAction::triggered,
                   [&handler]() { handler.SaveCurrentProject(); });
  return result;
}

/**
 * @brief Creates an action to trigger "save project as" request.
 */
std::unique_ptr<QAction> CreateSaveProjectAsAction(sup::gui::IProjectHandler &handler)
{
  auto result = std::make_unique<QAction>("Save &As...");
  result->setShortcuts(QKeySequence::SaveAs);
  result->setStatusTip("Save project under different name");
  QObject::connect(result.get(), &QAction::triggered, [&handler]() { handler.SaveProjectAs(); });
  return result;
}

/**
 * @brief Adds action to the menu, transfer ownership.
 */
void AddAction(QMenu *menu, std::unique_ptr<QAction> action)
{
  action->setParent(menu);  // transfer ownerhip
  menu->addAction(action.release());
}

}  // namespace

namespace sup::gui
{

void AddNewProjectAction(QMenu *menu, IProjectHandler &handler)
{
  AddAction(menu, CreateNewProjectAction(handler));
}

void AddOpenExistingProjectAction(QMenu *menu, IProjectHandler &handler)
{
  AddAction(menu, CreateOpenExistingProjectAction(handler));
}

void AddSaveCurrentProjectAction(QMenu *menu, IProjectHandler &handler)
{
  AddAction(menu, CreateSaveCurrentProjectAction(handler));
}

void AddSaveProjectAsAction(QMenu *menu, IProjectHandler &handler)
{
  AddAction(menu, CreateSaveProjectAsAction(handler));
}

void AddRecentProjectActions(QMenu *menu, IProjectHandler &handler)
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
    QAction::connect(action, &QAction::triggered,
                     [&handler]() { handler.ClearRecentProjectsList(); });
  }
}

}  // namespace sup::gui
