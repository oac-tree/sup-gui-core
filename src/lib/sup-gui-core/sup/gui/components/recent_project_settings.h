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

#ifndef SUP_GUI_COMPONENTS_RECENT_PROJECT_SETTINGS_H_
#define SUP_GUI_COMPONENTS_RECENT_PROJECT_SETTINGS_H_

#include <QStringList>

namespace sup::gui
{

/**
 * @brief Collection of settings to save the last directory selected by the user, and list of recent
 * projects.
 */

class RecentProjectSettings
{
public:
  RecentProjectSettings(const QString& group_name = "RecentProjectSettings");
  ~RecentProjectSettings();

  /**
   * @brief Returns current working directory.
   */
  QString GetCurrentWorkdir() const;

  /**
   * @brief Sets current working directory to a given value (full path).
   */
  void SetCurrentWorkdir(const QString& dir_name);

  /**
   * @brief Updates current working directory from the project directory.
   *
   * @param project_dir_name The directory of the project where we save results (full path).
   *
   * @details Our current convention is that the directory of the project is created in current
   * working directory.
   */
  void UpdateCurrentWorkdir(const QString& project_dir_name);

  /**
   * @brief Returns list of recent projects.
   *
   * @details The method also validates that projects still exists on disk.
   */
  QStringList GetRecentProjectList();

  /**
   * @brief Adds given project dir name (full path) to the list of recent projects.
   */
  void AddToRecentProjectList(const QString& project_dir_name);

  /**
   * @brief Clears the list of recent projects.
   */
  void ClearRecentProjectsList();

private:
  /**
   * @brief Writes settings to disk.
   */
  void WriteSettings();

  /**
   * @brief Reads settings from disk.
   */
  void ReadSettings();

  /**
   * @brief Validates if projects exist, and update the list to show only existing projects.
   */
  void ValidateIfProjectsExist();

  QString m_current_workdir;
  QStringList m_recent_projects;
  QString m_group_name; //!< the name of the group for persistent settings
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_RECENT_PROJECT_SETTINGS_H_
