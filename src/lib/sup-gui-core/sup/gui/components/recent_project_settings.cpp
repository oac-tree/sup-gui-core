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

#include "recent_project_settings.h"

#include <mvvm/utils/file_utils.h>

#include <QDir>
#include <QSettings>

namespace
{
const int kMaxRecentProjects = 10;

QString GetCurrentWorkdirSettingName(const QString& group_name)
{
  return group_name + "/" + "workdir";
}

QString GetRecentProjectsSettingName(const QString& group_name)
{
  return group_name + "/" + "recent_projects";
}

}  // namespace

namespace sup::gui
{

RecentProjectSettings::RecentProjectSettings(const QString& group_name) : m_group_name(group_name)
{
  ReadSettings();
}

RecentProjectSettings::~RecentProjectSettings()
{
  WriteSettings();
}

QString RecentProjectSettings::GetCurrentWorkdir() const
{
  return m_current_workdir;
}

void RecentProjectSettings::SetCurrentWorkdir(const QString& dir_name)
{
  m_current_workdir = dir_name;
}

void RecentProjectSettings::UpdateCurrentWorkdir(const QString& project_dir_name)
{
  if (!project_dir_name.isEmpty())
  {
    // working directory is a parent of project directory
    auto parent_path = mvvm::utils::GetParentPath(project_dir_name.toStdString());
    m_current_workdir = QString::fromStdString(parent_path);
  }
}

QStringList RecentProjectSettings::GetRecentProjectList()
{
  ValidateIfProjectsExist();
  return m_recent_projects;
}

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

void RecentProjectSettings::WriteSettings()
{
  ValidateIfProjectsExist();

  QSettings settings;
  settings.setValue(GetCurrentWorkdirSettingName(m_group_name), m_current_workdir);
  settings.setValue(GetRecentProjectsSettingName(m_group_name), m_recent_projects);
}

void RecentProjectSettings::ReadSettings()
{
  ValidateIfProjectsExist();

  QSettings settings;
  m_current_workdir =
      settings.value(GetCurrentWorkdirSettingName(m_group_name), QDir::homePath()).toString();
  m_recent_projects = settings.value(GetRecentProjectsSettingName(m_group_name)).toStringList();
}

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
