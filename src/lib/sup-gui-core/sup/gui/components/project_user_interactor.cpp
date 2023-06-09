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

#include "project_user_interactor.h"

#include "recent_project_settings.h"

#include <mvvm/project/project_types.h>
#include <mvvm/project/project_utils.h>
#include <mvvm/utils/file_utils.h>

#include <QFileDialog>
#include <QMessageBox>
#include <map>

#include <iostream>

namespace
{
//! Map of standard Qt answers to what ProjectManager expects.
std::map<QMessageBox::StandardButton, mvvm::SaveChangesAnswer> answer_map()
{
  std::map<QMessageBox::StandardButton, mvvm::SaveChangesAnswer> result = {
      {QMessageBox::Save, mvvm::SaveChangesAnswer::kSave},
      {QMessageBox::Discard, mvvm::SaveChangesAnswer::kDiscard},
      {QMessageBox::Cancel, mvvm::SaveChangesAnswer::kCancel}};
  return result;
}
}  // namespace

namespace sup::gui
{

ProjectUserInteractor::ProjectUserInteractor(QWidget* parent)
    : m_settings(std::make_unique<sup::gui::RecentProjectSettings>()), m_parent(parent)
{
}

ProjectUserInteractor::~ProjectUserInteractor() = default;

//! Returns directory on disk selected by the user via QFileDialog.
//! Checks if selected directory can be the project directory.

std::string ProjectUserInteractor::OnSelectDirRequest()
{
  auto dirname = SummonSelectDialog("Select project directory");

  if (dirname.empty())  // no valid selection
  {
    return {};
  }

  if (!mvvm::ProjectUtils::IsPossibleProjectDir(dirname))
  {
    QMessageBox msgBox;
    msgBox.setText("Selected directory doesn't look like a project directory, choose another one");
    msgBox.exec();
    return {};
  }

  return dirname;
}

//! Returns new directory on disk created by the user via QFileDialog.

std::string ProjectUserInteractor::OnCreateDirRequest()

{
  auto dirname = SummonSelectDialog("Create new directory");

  if (dirname.empty())  // no valid selection
  {
    return {};
  }

  if (!mvvm::utils::IsEmpty(dirname))
  {
    QMessageBox msgBox;
    msgBox.setText("The selected directory is not empty, choose another one.");
    msgBox.exec();
    return {};
  }

  return dirname;
}

//! Returns save/cancel/discard changes choice provided by the user.

mvvm::SaveChangesAnswer ProjectUserInteractor::OnSaveChangesRequest()
{
  static auto translate = answer_map();

  QMessageBox msgBox;
  msgBox.setText("The project has been modified.");
  msgBox.setInformativeText("Do you want to save your changes?");
  msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Save);
  auto ret = static_cast<QMessageBox::StandardButton>(msgBox.exec());
  return translate[ret];
}

//! Returns list of recent projects, validates if projects still exists on disk.

QStringList ProjectUserInteractor::GetRecentProjectList()
{
  return m_settings->GetRecentProjectList();
}

//! Adds directory to the list of recent projects.

void ProjectUserInteractor::AddToRecentProjectList(const QString& project_dir_name)
{
  return m_settings->AddToRecentProjectList(project_dir_name);
}

void ProjectUserInteractor::ClearRecentProjectsList()
{
  return m_settings->ClearRecentProjectsList();
}

void ProjectUserInteractor::SetDontUseNativeDialog(bool value)
{
  m_dont_use_native_dialogs = value;
}

//! Summon dialog to select directory on disk. If selection is not empty,
//! save parent directory for later re-use.

std::string ProjectUserInteractor::SummonSelectDialog(const QString& title) const
{
  std::cout << "AAAA " << m_dont_use_native_dialogs << std::endl;
  QFileDialog dialog(m_parent, title, m_settings->GetCurrentWorkdir());
  dialog.setFileMode(QFileDialog::Directory);
  dialog.setOption(QFileDialog::DontUseNativeDialog, m_dont_use_native_dialogs);
  dialog.setOption(QFileDialog::DontResolveSymlinks);
  dialog.setOption(QFileDialog::ShowDirsOnly);
  QStringList file_names = dialog.exec() ? dialog.selectedFiles() : QStringList();

  QString dirname = file_names.empty() ? QString() : file_names.at(0);

  if (!dirname.isEmpty())
  {
    m_settings->UpdateCurrentWorkdir(dirname);
  }

  return dirname.toStdString();
}

}  // namespace sup::gui
