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

#include "file_based_user_interactor.h"

#include <QFileDialog>

namespace sup::gui
{

FileBasedUserInteractor::FileBasedUserInteractor(QWidget* parent) : m_parent(parent) {}

std::string FileBasedUserInteractor::GetNewProjectPathImpl() const
{
  auto workdir = QString::fromStdString(GetCurrentWorkdir());
  auto file_name =
      QFileDialog::getSaveFileName(m_parent, "Save File", workdir, "XML files (*.xml *.XML)");

  return file_name.toStdString();
}

std::string FileBasedUserInteractor::GetExistingProjectPathImpl() const
{
  auto workdir = QString::fromStdString(GetCurrentWorkdir());
  auto file_name =
      QFileDialog::getOpenFileName(m_parent, "Open file", workdir, "Files (*.xml *.XML");

  return file_name.toStdString();
}

}  // namespace sup::gui
