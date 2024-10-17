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

#ifndef SUP_GUI_PROJECT_I_PROJECT_HANDLER_H_
#define SUP_GUI_PROJECT_I_PROJECT_HANDLER_H_

#include <QString>

namespace sup::gui
{

/**
 * @brief The ProjectHandler class is an interface to handle all user action requests to create a
 * new project, save it, open the existing one, or choose one of the recent projects on disk.
 */
class IProjectHandler
{
public:
  virtual ~IProjectHandler() = default;

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
  virtual bool CloseCurrentProject() = 0;

  /**
   * @brief Creates new project.
   */
  virtual void CreateNewProject() = 0;

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
  virtual void OpenExistingProject(const QString& path) = 0;

  /**
   * @brief Saves current project.
   *
   * The project should have a project path defined to succeed. If it is not the case, it will
   * launch the procedure of path selection.
   */
  virtual void SaveCurrentProject() = 0;

  /**
   * @brief Summon dialog to select new project path and save project to it.
   */
  virtual void SaveProjectAs() = 0;

  /**
   * @brief Clears the list of recent projects.
   */
  virtual void ClearRecentProjectsList() = 0;

  /**
   * @brief Returns the list of recent projects.
   */
  virtual QStringList GetRecentProjectList() const = 0;
};

}  // namespace sup::gui

#endif  // SUP_GUI_PROJECT_I_PROJECT_HANDLER_H_
