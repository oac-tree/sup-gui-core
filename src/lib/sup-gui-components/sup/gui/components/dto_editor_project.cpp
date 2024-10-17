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

#include "dto_editor_project.h"

#include <sup/gui/app/app_constants.h>
#include <sup/gui/model/sup_dto_model.h>
#include <sup/gui/model/waveform_model.h>

#include <mvvm/model/application_model.h>

namespace sup::gui
{

namespace
{

/**
 * @brief Creates context to pass to AbstractProject.
 */
mvvm::ProjectContext CreateContext(DtoEditorProject::callback_t modified_callback,
                                   DtoEditorProject::callback_t loaded_callback)
{
  return {std::move(modified_callback), std::move(loaded_callback),
          constants::kDtoEditorApplicationType.toStdString()};
}

}  // namespace

DtoEditorProject::DtoEditorProject(callback_t modified_callback, callback_t loaded_callback)
    : AppProject(CreateContext(std::move(modified_callback), std::move(loaded_callback)))
    , m_sup_dto_model_index(RegisterModel<SupDtoModel>())
    , m_waveform_model_index(RegisterModel<WaveformModel>())
{
}

DtoEditorProject::~DtoEditorProject() = default;

SupDtoModel *DtoEditorProject::GetSupDtoModel()
{
  return GetModelCount() > m_sup_dto_model_index ? GetModel<SupDtoModel>(m_sup_dto_model_index)
                                                 : nullptr;
}

WaveformModel *DtoEditorProject::GetWaveformModel()
{
  return GetModelCount() > m_waveform_model_index ? GetModel<WaveformModel>(m_waveform_model_index)
                                                  : nullptr;
}

}  // namespace sup::gui
