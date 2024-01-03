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

namespace anyvalueeditor
{

class AnyValueEditorMainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit AnyValueEditorMainWindowActions(QMainWindow* mainwindow = nullptr);

signals:
  void OnImportFromFileRequest();
  void OnExportToFileRequest();
  void RestartApplicationRequest(sup::gui::AppExitCode);

private:
  void CreateActions(QMainWindow* mainwindow);
  void SetupMenus(QMenuBar* menubar);

  void OnChangeSystemFont();
  void OnResetSettings();
  void OnAbout();

  QAction* m_import_action{nullptr};
  QAction* m_export_action{nullptr};

  QAction* m_system_font_action{nullptr};
  QAction* m_reset_settings_action{nullptr};

  QAction* m_exit_action{nullptr};

  QAction* m_about_action{nullptr};
};

}  // namespace anyvalueeditor

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_MAIN_WINDOW_ACTIONS_H_
