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

#ifndef SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_MAIN_WINDOW_ACTIONS_H_
#define SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_MAIN_WINDOW_ACTIONS_H_

#include <sup/gui/app/main_window_types.h>

#include <QObject>

class QMainWindow;
class QMenuBar;
class QAction;
class QMenu;

namespace mvvm
{
class SessionModelInterface;
}

namespace sup::gui
{

class ProjectHandler;

class AnyValueEditorMainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit AnyValueEditorMainWindowActions(mvvm::SessionModelInterface* model,
                                           QMainWindow* mainwindow = nullptr);

  bool CloseCurrentProject() const;

signals:
  void OnImportFromFileRequest();
  void OnExportToFileRequest();
  void RestartApplicationRequest(sup::gui::AppExitCode);
  void ProjectLoaded();

private:
  void CreateActions(QMainWindow* mainwindow);
  void SetupMenus();
  void SetupFileMenu();
  void SetupEditMenu();
  void SetupViewMenu();
  void SetupHelpMenu();

  void OnChangeSystemFont();
  void OnResetSettings();
  void OnAbout();

  QAction* m_import_action{nullptr};
  QAction* m_export_action{nullptr};
  QAction* m_system_font_action{nullptr};
  QAction* m_reset_settings_action{nullptr};
  QAction* m_exit_action{nullptr};
  QAction* m_about_action{nullptr};

  QMenu* m_recent_project_menu{nullptr};

  sup::gui::ProjectHandler* m_project_handler{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_MAIN_WINDOW_ACTIONS_H_
