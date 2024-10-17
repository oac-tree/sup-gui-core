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

#include <sup/gui/app/app_constants.h>

#include <mvvm/model/application_model.h>

namespace sup::gui
{

namespace
{

/**
 * @brief Creates context to pass to AbstractProject.
 */
mvvm::ProjectContext CreateContext(AnyValueEditorProject::callback_t modified_callback,
                                   AnyValueEditorProject::callback_t loaded_callback)
{
  return {std::move(modified_callback), std::move(loaded_callback),
          constants::kAnyValueEditorApplicationType.toStdString()};
}

}  // namespace

AnyValueEditorProject::AnyValueEditorProject(callback_t modified_callback,
                                             callback_t loaded_callback)
    : AppProject(CreateContext(std::move(modified_callback), std::move(loaded_callback)))
{
  RegisterModel<mvvm::ApplicationModel>();
}

AnyValueEditorProject::~AnyValueEditorProject() = default;

mvvm::ApplicationModel *AnyValueEditorProject::GetApplicationModel()
{
  return GetModelCount() > 0 ? GetModel<mvvm::ApplicationModel>() : nullptr;
}

}  // namespace sup::gui
