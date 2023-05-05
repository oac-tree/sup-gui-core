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

#ifndef SUP_GUI_COMPONENTS_PROJECT_HANDLER_H_
#define SUP_GUI_COMPONENTS_PROJECT_HANDLER_H_

#include <QObject>
#include <memory>
#include <vector>

class QWidget;

namespace mvvm
{
class ProjectManagerInterface;
class SessionModelInterface;
}  // namespace mvvm

namespace sup::gui
{

class UserInteractor;
class ApplicationModels;
class RecentProjectWidget;
class RecentProjectSettings;

//! Main class to coordinate all activity on user's request to create new project,
//! open existing one, or choose one of recent projects on disk.

class ProjectHandler : public QObject
{
  Q_OBJECT

public:
  explicit ProjectHandler(mvvm::SessionModelInterface* model, QWidget* parent);
  ~ProjectHandler() override;

  bool CanCloseProject() const;
  void UpdateNames();
  void OnCreateNewProject();
  void OnOpenExistingProject(const QString& dirname = {});
  void OnSaveCurrentProject();
  void OnSaveProjectAs();

  void ClearRecentProjectsList();

  QStringList GetRecentProjectList() const;

signals:
  void CurrentProjectModified(const QString& project_dir, bool is_modified);
  void RecentProjectsListModified(const QStringList& projects);

private:
  void InitProjectManager();
  void UpdateCurrentProjectName();
  void UpdateRecentProjectNames();

  std::unique_ptr<sup::gui::RecentProjectSettings> m_recent_project_settings;
  std::unique_ptr<UserInteractor> m_user_interactor;
  std::unique_ptr<mvvm::ProjectManagerInterface> m_project_manager;
  mvvm::SessionModelInterface* m_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_PROJECT_HANDLER_H_
