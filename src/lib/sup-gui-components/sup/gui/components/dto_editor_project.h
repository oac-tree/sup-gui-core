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

#ifndef SUP_GUI_COMPONENTS_DTO_EDITOR_PROJECT_H_
#define SUP_GUI_COMPONENTS_DTO_EDITOR_PROJECT_H_

#include <sup/gui/app/app_project.h>

namespace sup::gui
{

class SupDtoModel;
class WaveformModel;

/**
 * @brief The DtoEditorProject class is a main project for sup-dto-editor application.
 *
 * It owns two models and belongs to main window.
 */
class DtoEditorProject : public AppProject
{
public:
  /**
   * @brief Main c-tor.
   *
   * @param modified_callback A callback to report when project was modified.
   * @param loaded_callback A callback to report when project was reloaded.
   */
  explicit DtoEditorProject(callback_t modified_callback, callback_t loaded_callback);

  ~DtoEditorProject() override;

  SupDtoModel* GetSupDtoModel();

  WaveformModel* GetWaveformModel();

private:
  size_t m_sup_dto_model_index;
  size_t m_waveform_model_index;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_DTO_EDITOR_PROJECT_H_
