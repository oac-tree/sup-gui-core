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

#include "project_handler.h"

#include "project_user_interactor.h"

#include <mvvm/factories/project_manager_factory.h>
#include <mvvm/project/project_types.h>
#include <mvvm/project/project_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QMainWindow>

namespace sup::gui
{

ProjectHandler::ProjectHandler(mvvm::SessionModelInterface* model, QWidget* parent)
    : QObject(parent)
    , m_user_interactor(std::make_unique<ProjectUserInteractor>(parent))
    , m_model(model)
{
  InitProjectManager();
  UpdateNames();
}

ProjectHandler::~ProjectHandler() = default;

//! Closes curent project, returns true in the case of success. Internally will perform check for
//! unsaved data, and proceed via save/discard/cancel dialog.

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

void ProjectHandler::OpenExistingProject(const QString& dirname)
{
  if (m_project_manager->OpenExistingProject(dirname.toStdString()))
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

//! Performs saving of the project under different name. The name will be asked at the later stage
//! via corresponding dialog.

void ProjectHandler::SaveProjectAs()
{
  if (m_project_manager->SaveProjectAs({}))
  {
    UpdateNames();
  }
}

void ProjectHandler::ClearRecentProjectsList()
{
  m_user_interactor->ClearRecentProjectsList();
  UpdateNames();
}

QStringList ProjectHandler::GetRecentProjectList() const
{
  return m_user_interactor->GetRecentProjectList();
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

  auto select_dir_callback = [this]() { return m_user_interactor->OnSelectDirRequest(); };
  auto create_dir_callback = [this]() { return m_user_interactor->OnCreateDirRequest(); };
  auto answer_callback = [this]() { return m_user_interactor->OnSaveChangesRequest(); };
  mvvm::UserInteractionContext user_context{select_dir_callback, create_dir_callback,
                                            answer_callback};

  m_project_manager = CreateProjectManager(project_context, user_context);
}

//! Performs internal updates related to project name change.

void ProjectHandler::UpdateNames()
{
  UpdateCurrentProjectName();
  UpdateRecentProjectNames();
}

//! Updates the name of the current project on main window.

void ProjectHandler::UpdateCurrentProjectName()
{
  const auto current_project_dir = m_project_manager->CurrentProjectDir();
  const auto is_modified = m_project_manager->IsModified();

  // set main window title
  auto title = mvvm::utils::ProjectWindowTitle(current_project_dir, is_modified);
  if (auto main_window = mvvm::utils::FindMainWindow(); main_window)
  {
    main_window->setWindowTitle(QString::fromStdString(title));
  }
}

//! Update recent project list in settings.

void ProjectHandler::UpdateRecentProjectNames()
{
  m_user_interactor->AddToRecentProjectList(
      QString::fromStdString(m_project_manager->CurrentProjectDir()));
}

}  // namespace sup::gui
