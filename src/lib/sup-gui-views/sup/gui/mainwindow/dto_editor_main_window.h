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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_MAINWINDOW_DTO_EDITOR_MAIN_WINDOW_H_
#define SUP_GUI_MAINWINDOW_DTO_EDITOR_MAIN_WINDOW_H_

#include <sup/gui/app/main_window_types.h>
#include <sup/gui/components/dto_editor_project.h>

#include <QMainWindow>
#include <memory>

class QCloseEvent;

namespace mvvm
{
class MainVerticalBarWidget;
}  // namespace mvvm

namespace sup::gui
{

class DtoComposerView;
class DtoEditorMainWindowActions;
class DtoWaveformView;
class DtoEditorProject;
class SettingsModel;

/**
 * @brief The DtoEditorMainWindow class represents a main window of sup-dto-editor application.
 */
class DtoEditorMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  DtoEditorMainWindow();
  ~DtoEditorMainWindow() override;

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void InitApplication();
  void InitComponents();
  void ReadSettings();
  void WriteSettings();

  /**
   * @brief Check if the application can be closed.
   *
   * This saves unsaved projects and writes persistent application settings.
   *
   * @return True if the application is ready to be closed.
   */
  bool CanCloseApplication();

  /**
   * @brief Check if the application can be restarted and exit the application with the given code.
   *
   * The code will be used to start the application again from the main.
   */
  void OnRestartRequest(sup::gui::AppExitCode exit_code);

  /**
   * @brief Perform widgets setup on new project creation or project load from disk.
   */
  void OnProjectLoad();

  /**
   * @brief Perform widgets setup on project modification.
   */
  void UpdateProjectNames();

  /**
   * @brief Creates main application project agent.
   */
  std::unique_ptr<DtoEditorProject> CreateProject();

  std::unique_ptr<SettingsModel> m_settings;
  std::unique_ptr<DtoEditorProject> m_project;
  mvvm::MainVerticalBarWidget* m_tab_widget{nullptr};
  DtoEditorMainWindowActions* m_action_manager{nullptr};
  DtoComposerView* m_composer_view{nullptr};
  DtoWaveformView* m_waveform_view{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_DTO_EDITOR_MAIN_WINDOW_H_
