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

#include <mvvm/factories/model_document_factory.h>
#include <mvvm/model/application_model.h>
#include <mvvm/serialization/xml_document_helper.h>

#include <QString>

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
    : AbstractProject(mvvm::ProjectType::kFileBased,
                      CreateContext(std::move(modified_callback), std::move(loaded_callback)))
{
}

mvvm::ApplicationModel *AnyValueEditorProject::GetApplicationModel()
{
  return m_model ? m_model.get() : nullptr;
}

AnyValueEditorProject::~AnyValueEditorProject() = default;

std::vector<mvvm::ISessionModel *> AnyValueEditorProject::GetModels() const
{
  std::vector<mvvm::ISessionModel *> result;
  if (m_model)
  {
    result.push_back(m_model.get());
  }
  return result;
}

bool AnyValueEditorProject::SaveImpl(const std::string &path)
{
  auto document = mvvm::CreateXmlDocument(GetModels(), GetApplicationType());
  document->Save(path);
  return true;
}

bool AnyValueEditorProject::LoadImpl(const std::string &path)
{
  auto models = mvvm::LoadModels<mvvm::ApplicationModel>(path, GetApplicationType());
  m_model = std::move(std::get<0>(models));
  return true;
}

bool AnyValueEditorProject::CloseProjectImpl()
{
  m_model.reset();
  return true;
}

bool AnyValueEditorProject::CreateNewProjectImpl()
{
  m_model = std::make_unique<mvvm::ApplicationModel>();
  return true;
}

}  // namespace sup::gui
