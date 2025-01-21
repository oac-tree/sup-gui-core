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

#ifndef SUP_GUI_MAINWINDOW_ANYVALUE_EDITOR_MAIN_WINDOW_ACTIONS_H_
#define SUP_GUI_MAINWINDOW_ANYVALUE_EDITOR_MAIN_WINDOW_ACTIONS_H_

#include <sup/gui/app/main_window_types.h>

#include <QObject>
#include <memory>

class QMainWindow;
class QMenuBar;
class QAction;
class QMenu;
class QStatusBar;
class QToolButton;

namespace mvvm
{
class IProject;
class ProjectHandler;
}  // namespace mvvm

namespace sup::gui
{

class AppContextFocusController;

/**
 * @brief The AnyValueEditorMainWindowActions class handles actions of the main menubar.
 */
class AnyValueEditorMainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit AnyValueEditorMainWindowActions(mvvm::IProject* project,
                                           QMainWindow* mainwindow = nullptr);
  ~AnyValueEditorMainWindowActions() override;

  /**
   * @brief Closes current project.
   *
   * Internally performs check for unsaved data, and proceeds via save/discard/cancel dialog.
   * Returns true if project was successfully saved or user has agreed to discard unsaved changes,
   * and false otherwise. The later normally means that the user has changed his mind in the course
   * of this operation, canceled dialog, and the project has remained in unsaved state.
   *
   * @return True in the case of success.
   */
  bool CloseCurrentProject() const;

  /**
   * @brief Update currently opened project name.
   *
   * Handles modified status of project name, updates recent project list.
   */
  void UpdateProjectNames();

  /**
   * @brief Populates external status bar with actions.
   */
  void SetupStatusBar(QStatusBar* status_bar);

signals:
  void OnImportFromFileRequest();
  void OnExportToFileRequest();
  void RestartApplicationRequest(sup::gui::AppExitCode);
  void ProjectLoaded();
  void OnImportWaveformRequest();

private:
  void CreateActions(QMainWindow* mainwindow);
  void SetupMenus();
  void SetupFileMenu();
  void SetupEditMenu();
  void SetupViewMenu();
  void SetupHelpMenu();

  void OnChangeSystemFont();
  void OnApplicationSettingsDialog();
  void OnResetSettings();
  void OnAbout();

  QAction* m_import_action{nullptr};
  QAction* m_import_waveform_action{nullptr};
  QAction* m_export_action{nullptr};
  QAction* m_system_font_action{nullptr};
  QAction* m_settings_dialog_action{nullptr};
  QAction* m_reset_settings_action{nullptr};
  QAction* m_exit_action{nullptr};
  QAction* m_about_action{nullptr};

  QMenu* m_recent_project_menu{nullptr};
  QToolButton* m_toggle_right_sidebar_button{nullptr};

  std::unique_ptr<mvvm::ProjectHandler> m_project_handler;
  std::unique_ptr<sup::gui::AppContextFocusController> m_focus_controller;
};

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_ANYVALUE_EDITOR_MAIN_WINDOW_ACTIONS_H_
