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

#include "abstract_project_user_interactor.h"

#include <mvvm/utils/file_utils.h>

namespace sup::gui
{

std::string AbstractProjectUserInteractor::GetCurrentWorkdir() const
{
  return m_current_workdir;
}

void AbstractProjectUserInteractor::SetCurrentWorkdir(const std::string &path)
{
  m_current_workdir = path;
}

std::string AbstractProjectUserInteractor::GetNewProjectPath(mvvm::ProjectType project_type) const
{
  (void)project_type;
  auto result = GetNewProjectPathImpl();
  UpdateCurrentWorkdir(result);
  return result;
}

std::string AbstractProjectUserInteractor::GetExistingProjectPath(
    mvvm::ProjectType project_type) const
{
  (void)project_type;
  auto result = GetExistingProjectPathImpl();
  UpdateCurrentWorkdir(result);
  return result;
}

void AbstractProjectUserInteractor::UpdateCurrentWorkdir(const std::string &path) const
{
  if (!path.empty())
  {
    // updating mutable variable from const method
    m_current_workdir = mvvm::utils::GetParentPath(path);
  }
}

}  // namespace sup::gui
