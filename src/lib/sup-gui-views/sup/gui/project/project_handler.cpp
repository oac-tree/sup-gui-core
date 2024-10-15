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

#include "file_based_user_interactor.h"
#include "folder_based_user_interactor.h"
#include "recent_project_settings.h"

#include <mvvm/project/i_project.h>
#include <mvvm/project/project_context.h>
#include <mvvm/project/project_manager_factory.h>
#include <mvvm/project/project_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QDebug>
#include <QMainWindow>

namespace
{

std::unique_ptr<sup::gui::AbstractProjectUserInteractor> CreateUserInteractor(
    mvvm::ProjectType project_type, const QString& application_type, QWidget* parent)
{
  if (project_type == mvvm::ProjectType::kFolderBased)
  {
    return std::make_unique<sup::gui::FolderBasedUserInteractor>(application_type, parent);
  }
  return std::make_unique<sup::gui::FileBasedUserInteractor>(application_type, parent);
}

}  // namespace

namespace sup::gui
{

ProjectHandler::ProjectHandler(mvvm::ProjectType project_type, const QString& application_type,
                               const std::vector<mvvm::ISessionModel*>& models, QWidget* parent)
    : QObject(parent)
    , m_project_type(project_type)
    , m_application_type(application_type)
    , m_user_interactor(CreateUserInteractor(project_type, application_type, parent))
    , m_recent_projects(std::make_unique<RecentProjectSettings>())
    , m_models(models)
    , m_project(mvvm::utils::CreateUntitledProject(m_project_type, models, CreateProjectContext()))
{
  InitProjectManager();
  UpdateNames();

  m_user_interactor->SetCurrentWorkdir(m_recent_projects->GetCurrentWorkdir().toStdString());
}

ProjectHandler::~ProjectHandler()
{
  m_recent_projects->SetCurrentWorkdir(
      QString::fromStdString(m_user_interactor->GetCurrentWorkdir()));
}

bool ProjectHandler::CloseCurrentProject()
{
  auto result = m_project_manager->CloseCurrentProject();
  if (result)
  {
    UpdateNames();
  }
  return result;
}

void ProjectHandler::CreateNewProject()
{
  if (m_project_manager->CreateNewProject({}))
  {
    UpdateNames();
    emit ProjectLoaded();
  }
}

void ProjectHandler::OpenExistingProject(const QString& path)
{
  if (m_project_manager->OpenExistingProject(path.toStdString()))
  {
    UpdateNames();
    emit ProjectLoaded();
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
  m_project_manager = CreateProjectManager(*m_project, m_user_interactor->CreateContext());
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

mvvm::ProjectContext ProjectHandler::CreateProjectContext()
{
  mvvm::ProjectContext result;
  result.modified_callback = [this]() { UpdateCurrentProjectName(); };
  result.application_type = m_application_type.toStdString();
  return result;
}

}  // namespace sup::gui
