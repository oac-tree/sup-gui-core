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

namespace sup::gui
{

class AnyValueEditor;
class AnyValueEditorMainWindowActions;

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
  void OnProjectLoad();

  /**
   * @brief Checks if application can be closed.
   *
   * This perform saving of unsaved projects, and writing persistent application settings.
   *
   * @return True if application is ready to be closed.
   */
  bool CanCloseApplication();

  void OnRestartRequest(sup::gui::AppExitCode exit_code);

  AnyValueEditorMainWindowActions* m_action_manager{nullptr};
  sup::gui::AnyValueEditor* m_anyvalue_editor{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_ANYVALUE_EDITOR_MAIN_WINDOW_H_
