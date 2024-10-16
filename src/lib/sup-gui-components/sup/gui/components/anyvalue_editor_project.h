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

#ifndef SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_PROJECT_H_
#define SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_PROJECT_H_

#include <mvvm/project/abstract_project.h>

namespace mvvm
{
class ApplicationModel;
}

namespace sup::gui
{

/**
 * @brief The AnyValueEditorProject class is a main project for anyvalue-editor application.
 *
 * It owns a single ApplicationModel for AnyValue editing. Belongs to AnyValueEditorMainWindow.
 */
class AnyValueEditorProject : public mvvm::AbstractProject
{
public:
  using callback_t = std::function<void()>;

  /**
   * @brief Main c-tor.
   *
   * @param modified_callback A callback to report when project was modified.
   * @param loaded_callback A callback to report when project was reloaded.
   */
  explicit AnyValueEditorProject(callback_t modified_callback, callback_t loaded_callback);

  ~AnyValueEditorProject() override;

  mvvm::ApplicationModel* GetApplicationModel();

  std::vector<mvvm::ISessionModel*> GetModels() const override;

private:
  bool SaveImpl(const std::string& path) override;
  bool LoadImpl(const std::string& path) override;
  bool CloseProjectImpl() override;
  bool CreateNewProjectImpl() override;

  std::unique_ptr<mvvm::ApplicationModel> m_model;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_PROJECT_H_
