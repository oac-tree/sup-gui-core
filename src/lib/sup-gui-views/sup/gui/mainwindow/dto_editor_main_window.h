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

#ifndef SUP_GUI_MAINWINDOW_DTO_EDITOR_MAIN_WINDOW_H_
#define SUP_GUI_MAINWINDOW_DTO_EDITOR_MAIN_WINDOW_H_

#include <sup/gui/components/dto_editor_project.h>
#include <sup/gui/settings/scoped_callback.h>

#include <mvvm/widgets/main_window_types.h>

#include <QMainWindow>
#include <memory>

class QCloseEvent;

namespace mvvm
{
class MainVerticalBarWidget;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueEditorAppContext;
class DtoCompareView;
class DtoComposerView;
class DtoEditorMainWindowActions;
class DtoEditorProject;
class DtoExplorerView;
class DtoWaveformView;
class IAppCommandService;
class PersistentSettingsModel;

/**
 * @brief The DtoEditorMainWindow class represents a main window of sup-dto-editor application.
 */
class DtoEditorMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit DtoEditorMainWindow(AnyValueEditorAppContext& context);
  ~DtoEditorMainWindow() override;

  DtoEditorMainWindow(const DtoEditorMainWindow&) = delete;
  DtoEditorMainWindow& operator=(const DtoEditorMainWindow&) = delete;
  DtoEditorMainWindow(DtoEditorMainWindow&&) = delete;
  DtoEditorMainWindow& operator=(DtoEditorMainWindow&&) = delete;

  /**
   * @brief Imports AnyValue from JSON file.
   */
  void ImportAnyValueFromFile(const QString& file_name);

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
   * @brief Handle application exit request.
   */
  void OnExitRequest(mvvm::AppExitRequest exit_request);

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

  AnyValueEditorAppContext& m_context;
  std::unique_ptr<PersistentSettingsModel> m_settings;
  std::unique_ptr<DtoEditorProject> m_project;
  mvvm::MainVerticalBarWidget* m_tab_widget{nullptr};
  DtoEditorMainWindowActions* m_action_manager{nullptr};
  DtoExplorerView* m_explorer_view{nullptr};
  DtoCompareView* m_compare_view{nullptr};
  DtoComposerView* m_composer_view{nullptr};
  DtoWaveformView* m_waveform_view{nullptr};
  ScopedCallback m_window_settings_connection;
};

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_DTO_EDITOR_MAIN_WINDOW_H_
