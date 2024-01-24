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

#include <QObject>
#include <memory>

class QWidget;

namespace mvvm
{
class ProjectManagerInterface;
class SessionModelInterface;
}  // namespace mvvm

namespace sup::gui
{

class ProjectUserInteractor;

/**
 * @brief The ProjectHandler class coordinates all the activity on the user's request to
 * create a new project, open the existing one, or choose one of the recent projects on disk.
 */

class ProjectHandler : public QObject
{
  Q_OBJECT

public:
  explicit ProjectHandler(mvvm::SessionModelInterface* model, QWidget* parent);
  ~ProjectHandler() override;

  /**
   * @brief Closes current project.
   *
   * @return True in the case of success.
   *
   * @details Internally performs check for unsaved data, and proceeds via save/discard/cancel
   * dialog. Returns true if project was successfully saved, and false otherwise. The later normally
   * means that the user has changed his mind in the course of this operation, canceled dialog, and
   * the project has remained in unsaved state.
   */
  bool CloseCurrentProject() const;

  /**
   * @brief Creates new project.
   */
  void CreateNewProject();

  /**
   * @brief Opens existing project.
   *
   * @param dirname The full path to the project directory.
   * @return Returns true in the case of success.
   *
   * @details If provided name is empty, will call directory selector dialog using callback
   * provided. If current project is in unsaved state, it will perform 'save-before-closing'
   * procedure before proceeding further.
   */
  void OpenExistingProject(const QString& dirname = {});

  /**
   * @brief Saves current project.
   * @return True in the case of success.
   *
   * @details The project should have a project directory defined to succeed. If it is not the case,
   * it will launch the procedure of directory selection.
   */
  void SaveCurrentProject();

  /**
   * @brief Summon dialog to select new project directory and save project to it.
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
   * @details When true, will use system native dialogs, if the system has one (default). When
   * false, will use Qt's own dialog.
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

  //!< knows how to interact with the user
  std::unique_ptr<ProjectUserInteractor> m_user_interactor;

  //!< performs actual model saving
  std::unique_ptr<mvvm::ProjectManagerInterface> m_project_manager;
  mvvm::SessionModelInterface* m_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_PROJECT_HANDLER_H_
