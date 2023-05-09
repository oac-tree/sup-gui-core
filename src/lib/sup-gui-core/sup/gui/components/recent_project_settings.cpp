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

#include "recent_project_settings.h"

#include <mvvm/utils/file_utils.h>

#include <QDir>
#include <QSettings>

namespace
{
const int kMaxRecentProjects = 10;

const QString kGroupName("RecentProjectSettings/");
const QString kCurrentWorkdirSettingName = kGroupName + "workdir";
const QString kRecentProjectsSettingName = kGroupName + "recent_projects";

}  // namespace

namespace sup::gui
{

RecentProjectSettings::RecentProjectSettings()
{
  ReadSettings();
}

RecentProjectSettings::~RecentProjectSettings()
{
  WriteSettings();
}

//! Returns current workdir.
QString RecentProjectSettings::GetCurrentWorkdir() const
{
  return m_current_workdir;
}

//! Updates current working directory.
void RecentProjectSettings::UpdateCurrentWorkdir(const QString& project_dir_name)
{
  if (!project_dir_name.isEmpty())
  {
    // working directory is a parent of project directory
    auto parent_path = mvvm::utils::GetParentPath(project_dir_name.toStdString());
    m_current_workdir = QString::fromStdString(parent_path);
  }
}

//! Returns list of recent projects, validates if projects still exists on disk.
QStringList RecentProjectSettings::GetRecentProjectList()
{
  ValidateIfProjectsExist();
  return m_recent_projects;
}

//! Adds directory to the list of recent projects.
void RecentProjectSettings::AddToRecentProjectList(const QString& project_dir_name)
{
  m_recent_projects.removeAll(project_dir_name);
  m_recent_projects.prepend(project_dir_name);
  while (m_recent_projects.size() > kMaxRecentProjects)
  {
    m_recent_projects.removeLast();
  }
}

void RecentProjectSettings::ClearRecentProjectsList()
{
  m_recent_projects.clear();
}

//! Write all settings to file.
void RecentProjectSettings::WriteSettings()
{
  ValidateIfProjectsExist();

  QSettings settings;
  settings.setValue(kCurrentWorkdirSettingName, m_current_workdir);
  settings.setValue(kRecentProjectsSettingName, m_recent_projects);
}

//! Reads all settings from file.
void RecentProjectSettings::ReadSettings()
{
  ValidateIfProjectsExist();

  QSettings settings;
  m_current_workdir = settings.value(kCurrentWorkdirSettingName, QDir::homePath()).toString();
  m_recent_projects = settings.value(kRecentProjectsSettingName).toStringList();
}

//! Validates if projects exist, and update the list to show only existing projects.

void RecentProjectSettings::ValidateIfProjectsExist()
{
  QStringList updated_list;
  for (const auto& file_name : m_recent_projects)
  {
    if (mvvm::utils::IsExists(file_name.toStdString()))
    {
      updated_list.append(file_name);
    }
  }
  m_recent_projects = updated_list;
}

}  // namespace sup::gui
