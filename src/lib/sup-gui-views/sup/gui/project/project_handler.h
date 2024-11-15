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

#ifndef SUP_GUI_PROJECT_PROJECT_HANDLER_H_
#define SUP_GUI_PROJECT_PROJECT_HANDLER_H_

#include <mvvm/project/i_project_manager.h>

#include <memory>

namespace mvvm
{
class IProject;
class IProjectManager;
}  // namespace mvvm

namespace sup::gui
{

class AbstractProjectUserInteractor;
class RecentProjectSettings;

class ProjectHandler : public mvvm::IProjectManager
{
public:
  explicit ProjectHandler(mvvm::IProject* project);

  ~ProjectHandler() override;

  bool CreateNewProject(const std::string& path) override;

  bool CloseProject() override;

  bool SaveCurrentProject() override;

  bool SaveProjectAs(const std::string& path) override;

  bool OpenExistingProject(const std::string& path) override;

  mvvm::IProject* GetProject() const override;

  void ClearRecentProjectsList() override;

  std::vector<std::string> GetRecentProjectList() const override;

  /**
   * @brief Performs internal updates related to project name change.
   */
  void UpdateNames();

private:
  /**
   * @brief Updates the name of the current project on main window.
   */
  void UpdateCurrentProjectName();

  /**
   * @brief Updates recent project list in settings.
   */
  void UpdateRecentProjectNames();

  //!< main project
  mvvm::IProject* m_project_agent{nullptr};

  //!< knows how to interact with the user
  std::unique_ptr<AbstractProjectUserInteractor> m_user_interactor;

  //!< performs actual model saving
  std::unique_ptr<mvvm::IProjectManager> m_project_manager;

  //!< tracks recent projects
  std::unique_ptr<sup::gui::RecentProjectSettings> m_recent_projects;
};

}  // namespace sup::gui

#endif  // SUP_GUI_PROJECT_PROJECT_HANDLER_H_
