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

#ifndef SUP_GUI_MAINWINDOW_ANYVALUE_EDITOR_MAIN_WINDOW_H_
#define SUP_GUI_MAINWINDOW_ANYVALUE_EDITOR_MAIN_WINDOW_H_

#include <sup/gui/app/main_window_types.h>

#include <QMainWindow>

class QCloseEvent;

namespace mvvm
{
class IProject;
}

namespace sup::gui
{

class AnyValueEditorWidget;
class AnyValueEditorMainWindowActions;
class AnyValueEditorProject;

/**
 * @brief The AnyValueEditorMainWindow class is a main window of anyvalue-editor application.
 */
class AnyValueEditorMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  AnyValueEditorMainWindow();
  ~AnyValueEditorMainWindow() override;

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

  void UpdateProjectNames();

  /**
   * @brief Creates main application project agent.
   */
  std::unique_ptr<AnyValueEditorProject> CreateProject();

  std::unique_ptr<AnyValueEditorProject> m_project;
  AnyValueEditorMainWindowActions* m_action_manager{nullptr};
  sup::gui::AnyValueEditorWidget* m_anyvalue_editor{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_ANYVALUE_EDITOR_MAIN_WINDOW_H_
