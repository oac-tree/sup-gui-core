/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

DtoEditorProject::DtoEditorProject(const mvvm::ProjectContext &context)
    : mvvm::AppProject(context)
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
