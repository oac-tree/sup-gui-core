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

#ifndef SUP_GUI_COMPONENTS_PROJECT_HANDLER_H_
#define SUP_GUI_COMPONENTS_PROJECT_HANDLER_H_

#include <mvvm/project/project_types.h>

#include <QObject>
#include <memory>

class QWidget;

namespace mvvm
{
class IProject;
class IProjectManager;
class SessionModelInterface;
}  // namespace mvvm

namespace sup::gui
{

class AbstractProjectUserInteractor;
class RecentProjectSettings;

/**
 * @brief The ProjectHandler class coordinates all the activity on the user's request to
 * create a new project, save it, open the existing one, or choose one of the recent projects on
 * disk.
 *
 * It is initialized with the list of models that will be used while saving and loading the
 * document. The same models will be listened to track if the document was modified since the last
 * save.
 *
 */
class ProjectHandler : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * @param models Collection of models which participate in save/load activity.
   * @param parent Parent widget.
   */
  explicit ProjectHandler(mvvm::ProjectType project_type,
                          const std::vector<mvvm::SessionModelInterface*>& models, QWidget* parent);
  ~ProjectHandler() override;

  /**
   * @brief Closes current project.
   *
   * Internally performs check for unsaved data, and proceeds via save/discard/cancel dialog.
   * Returns true if the project was successfully saved, and false otherwise. The later normally
   * means that the user has changed his mind in the course of this operation, canceled the dialog,
   * and the project has remained in an unsaved state.
   *
   * @return True in the case of success.
   */
  bool CloseCurrentProject() const;

  /**
   * @brief Creates new project.
   */
  void CreateNewProject();

  /**
   * @brief Opens existing project.
   *
   * If the provided name is empty, will call the path selector dialog using callback
   * provided. If the current project is in an unsaved state, it will perform 'save-before-closing'
   * procedure before proceeding further.
   *
   * @param path The full path to the project.
   * @return Returns true in the case of success.
   *
   * @details The path meaning depends on the type of the project. For file-based projects, it is a
   * full path to a project file, for the folder-based project, it is a full path to the project
   * directory.
   */
  void OpenExistingProject(const QString& path);

  /**
   * @brief Saves current project.
   *
   * The project should have a project path defined to succeed. If it is not the case, it will
   * launch the procedure of path selection.
   *
   * @return True in the case of success.
   */
  void SaveCurrentProject();

  /**
   * @brief Summon dialog to select new project path and save project to it.
   */
  void SaveProjectAs();

  /**
   * @brief Clears the list of recent projects.
   */
  void ClearRecentProjectsList();

  /**
   * @brief Returns the list of recent projects.
   */
  QStringList GetRecentProjectList() const;

  /**
   * @brief Sets the flag responsible for using system native file/directory selection dialogs.
   *
   * When true, will use system native dialogs, if the system has one (default). When false, will
   * use Qt's own dialog.
   */
  void SetUseNativeDialog(bool value);

private:
  void InitProjectManager();

  /**
   * @brief Performs internal updates related to project name change.
   */
  void UpdateNames();

  /**
   * @brief Updates the name of the current project on main window.
   */
  void UpdateCurrentProjectName();

  /**
   * @brief Updates recent project list in settings.
   */
  void UpdateRecentProjectNames();

  /**
   * @brief Creates new project.
   */
  std::unique_ptr<mvvm::IProject> CreateProject();

  //!< fixed type of projects which this handler generates
  mvvm::ProjectType m_project_type;

  //!< knows how to interact with the user
  std::unique_ptr<AbstractProjectUserInteractor> m_user_interactor;

  //!< performs actual model saving
  std::unique_ptr<mvvm::IProjectManager> m_project_manager;

  //!< tracks recent projects
  std::unique_ptr<sup::gui::RecentProjectSettings> m_recent_projects;

  std::vector<mvvm::SessionModelInterface*> m_models;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_PROJECT_HANDLER_H_
