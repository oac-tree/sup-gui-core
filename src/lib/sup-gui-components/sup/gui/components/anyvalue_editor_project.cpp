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

#include "anyvalue_editor_project.h"

#include <mvvm/model/application_model.h>

namespace sup::gui
{

// AnyValueEditorProject::AnyValueEditorProject(const AnyValueEditorProjectContext &context)
//     : AbstractProject(mvvm::ProjectType::kFileBased, context)
// {
// }

// AnyValueEditorProject::~AnyValueEditorProject() = default;

// std::vector<mvvm::ISessionModel *> AnyValueEditorProject::GetModels() const
// {
//   return {m_model.get()};
// }

// bool AnyValueEditorProject::SaveImpl(const std::string &path)
// {
//   return true;
// }

// bool AnyValueEditorProject::LoadImpl(const std::string &path)
// {
//   return true;
// }

// bool AnyValueEditorProject::CloseProjectImpl()
// {
//   m_model.reset();
//   return true;
// }

// bool AnyValueEditorProject::CreateNewProjectImpl()
// {
//   m_model = std::make_unique<mvvm::ApplicationModel>();
//   return true;
// }

}  // namespace sup::gui
