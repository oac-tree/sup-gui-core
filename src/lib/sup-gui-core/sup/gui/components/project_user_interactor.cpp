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

#include "project_user_interactor.h"

#include <mvvm/project/project_types.h>
#include <mvvm/project/project_utils.h>
#include <mvvm/utils/file_utils.h>

#include <QFileDialog>
#include <QMessageBox>

namespace sup::gui
{

FolderBasedUserInteractor::FolderBasedUserInteractor(QWidget* parent) : m_parent(parent) {}

std::string FolderBasedUserInteractor::GetNewProjectPathImpl() const
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

std::string FolderBasedUserInteractor::GetExistingProjectPathImpl() const
{
  auto dirname = SummonSelectDialog("Select project directory");

  if (dirname.empty())  // no valid selection
  {
    return {};
  }

  if (!mvvm::utils::IsPossibleProjectDir(dirname))
  {
    QMessageBox msgBox;
    msgBox.setText("Selected directory doesn't look like a project directory, choose another one");
    msgBox.exec();
    return {};
  }

  return dirname;
}

std::string FolderBasedUserInteractor::SummonSelectDialog(const QString& title) const
{
  QFileDialog dialog(m_parent, title, QString::fromStdString(GetCurrentWorkdir()));
  dialog.setFileMode(QFileDialog::Directory);
  dialog.setOption(QFileDialog::DontUseNativeDialog, !m_use_native_dialogs);
  dialog.setOption(QFileDialog::DontResolveSymlinks);
  dialog.setOption(QFileDialog::ShowDirsOnly);
  QStringList file_names = dialog.exec() ? dialog.selectedFiles() : QStringList();

  QString dirname = file_names.empty() ? QString() : file_names.at(0);
  return dirname.toStdString();
}

}  // namespace sup::gui
