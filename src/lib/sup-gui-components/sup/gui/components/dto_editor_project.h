/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_COMPONENTS_DTO_EDITOR_PROJECT_H_
#define SUP_GUI_COMPONENTS_DTO_EDITOR_PROJECT_H_

#include <mvvm/project/app_project.h>

namespace sup::gui
{

class SupDtoModel;
class WaveformModel;
class ConfigurationModel;

/**
 * @brief The DtoEditorProject class is a main project for sup-dto-editor application.
 *
 * It owns two models and belongs to main window.
 */
class DtoEditorProject : public mvvm::AppProject
{
public:
  /**
   * @brief Main c-tor.
   *
   * @param context Project context.
   */
  explicit DtoEditorProject(const mvvm::ProjectContext& context);

  ~DtoEditorProject() override;

  DtoEditorProject(const DtoEditorProject&) = delete;
  DtoEditorProject& operator=(const DtoEditorProject&) = delete;
  DtoEditorProject(DtoEditorProject&&) noexcept = delete;
  DtoEditorProject& operator=(DtoEditorProject&&) noexcept = delete;

  SupDtoModel* GetSupDtoModel();

  WaveformModel* GetWaveformModel();

  ConfigurationModel* GetConfigurationModel();

private:
  std::size_t m_sup_dto_model_index{0U};
  std::size_t m_waveform_model_index{0U};
  std::size_t m_configuration_model_index{0U};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_DTO_EDITOR_PROJECT_H_
