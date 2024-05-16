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

#include "project_handler.h"

#include "folder_based_user_interactor.h"
#include "recent_project_settings.h"

#include <mvvm/factories/project_manager_factory.h>
#include <mvvm/project/i_project.h>
#include <mvvm/project/project_context.h>
#include <mvvm/project/project_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QMainWindow>

namespace sup::gui
{

ProjectHandler::ProjectHandler(mvvm::SessionModelInterface* model, QWidget* parent)
    : QObject(parent)
    , m_user_interactor(std::make_unique<FolderBasedUserInteractor>(parent))
    , m_recent_projects(std::make_unique<RecentProjectSettings>())
    , m_model(model)
{
  InitProjectManager();
  UpdateNames();
}

ProjectHandler::~ProjectHandler() = default;

bool ProjectHandler::CloseCurrentProject() const
{
  return m_project_manager->CloseCurrentProject();
}

void ProjectHandler::CreateNewProject()
{
  if (m_project_manager->CreateNewProject({}))
  {
    UpdateNames();
  }
}

void ProjectHandler::OpenExistingProject(const QString& path)
{
  if (m_project_manager->OpenExistingProject(path.toStdString()))
  {
    UpdateNames();
  }
}

void ProjectHandler::SaveCurrentProject()
{
  if (m_project_manager->SaveCurrentProject())
  {
    UpdateNames();
  }
}

void ProjectHandler::SaveProjectAs()
{
  if (m_project_manager->SaveProjectAs({}))
  {
    UpdateNames();
  }
}

void ProjectHandler::ClearRecentProjectsList()
{
  m_recent_projects->ClearRecentProjectsList();
  UpdateNames();
}

QStringList ProjectHandler::GetRecentProjectList() const
{
  return m_recent_projects->GetRecentProjectList();
}

void ProjectHandler::SetUseNativeDialog(bool value)
{
  m_user_interactor->SetUseNativeDialog(value);
}

void ProjectHandler::InitProjectManager()
{
  auto modified_callback = [this]() { UpdateCurrentProjectName(); };
  auto models_callback = [this]() { return std::vector<mvvm::SessionModelInterface*>({m_model}); };
  mvvm::ProjectContext project_context{modified_callback, models_callback};

  auto project_factory_func = [project_context]()
  { return mvvm::utils::CreateUntitledFolderBasedProject(project_context); };

  auto user_context = m_user_interactor->CreateContext();

  m_project_manager = CreateProjectManager(project_factory_func, user_context);
}

void ProjectHandler::UpdateNames()
{
  UpdateCurrentProjectName();
  UpdateRecentProjectNames();
}

void ProjectHandler::UpdateCurrentProjectName()
{
  const auto current_project_dir = m_project_manager->CurrentProjectPath();
  const auto is_modified = m_project_manager->IsModified();

  // set main window title
  auto title = mvvm::utils::ProjectWindowTitle(current_project_dir, is_modified);
  if (auto main_window = mvvm::utils::FindMainWindow(); main_window)
  {
    main_window->setWindowTitle(QString::fromStdString(title));
  }
}

void ProjectHandler::UpdateRecentProjectNames()
{
  m_recent_projects->AddToRecentProjectList(
      QString::fromStdString(m_project_manager->CurrentProjectPath()));
}

}  // namespace sup::gui
