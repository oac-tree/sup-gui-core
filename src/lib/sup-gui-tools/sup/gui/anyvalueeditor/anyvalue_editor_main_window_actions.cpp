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

#include "anyvalue_editor_main_window_actions.h"

#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_action_manager.h>
#include <sup/gui/app/main_window_helper.h>
#include <sup/gui/components/project_handler.h>
#include <sup/gui/components/project_handler_utils.h>
#include <sup/gui/core/version.h>
#include <sup/gui/widgets/about_application_dialog.h>

#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QMainWindow>
#include <QMenuBar>

namespace sup::gui
{

AnyValueEditorMainWindowActions::AnyValueEditorMainWindowActions(mvvm::SessionModelInterface *model,
                                                                 QMainWindow *mainwindow)
    : QObject(mainwindow)
    , m_project_handler(
          new sup::gui::ProjectHandler(mvvm::ProjectType::kFileBased, {model}, mainwindow))
{
  CreateActions(mainwindow);
  SetupMenus();
}

bool AnyValueEditorMainWindowActions::CloseCurrentProject() const
{
  return m_project_handler->CloseCurrentProject();
}

void AnyValueEditorMainWindowActions::CreateActions(QMainWindow *mainwindow)
{
  m_import_action = new QAction("Import", this);
  m_import_action->setShortcuts(QKeySequence::Open);
  connect(m_import_action, &QAction::triggered, this,
          &AnyValueEditorMainWindowActions::OnImportFromFileRequest);

  m_export_action = new QAction("Export", this);
  m_export_action->setShortcuts(QKeySequence::Save);
  connect(m_export_action, &QAction::triggered, this,
          &AnyValueEditorMainWindowActions::OnExportToFileRequest);

  m_system_font_action = new QAction("System font (restart required)", this);
  m_system_font_action->setStatusTip("Summon font settings dialog");
  connect(m_system_font_action, &QAction::triggered, this,
          &AnyValueEditorMainWindowActions::OnChangeSystemFont);

  m_reset_settings_action = new QAction("Reset settings to defaults", this);
  m_reset_settings_action->setStatusTip(
      "Reset persistent application settings on disk to their defaults");
  m_reset_settings_action->setToolTip(
      "Reset persistent application settings on disk to their defaults");
  connect(m_reset_settings_action, &QAction::triggered, this,
          &AnyValueEditorMainWindowActions::OnResetSettings);

  m_exit_action = new QAction("E&xit Application", this);
  m_exit_action->setShortcuts(QKeySequence::Quit);
  m_exit_action->setStatusTip("Exit the application");
  connect(m_exit_action, &QAction::triggered, mainwindow, &QMainWindow::close);

  m_about_action = new QAction("About application", this);
  m_about_action->setStatusTip("About application");
  connect(m_about_action, &QAction::triggered, this, &AnyValueEditorMainWindowActions::OnAbout);
}

void AnyValueEditorMainWindowActions::SetupMenus()
{
  auto file_menu = AppGetMenu(constants::kFileMenu);

  AddNewProjectAction(file_menu, *m_project_handler);
  AddOpenExistingProjectAction(file_menu, *m_project_handler);

  m_recent_project_menu = file_menu->addMenu("Recent Projects");
  auto about_to_show_menu = [this]()
  { AddRecentProjectActions(m_recent_project_menu, *m_project_handler); };
  connect(file_menu, &QMenu::aboutToShow, this, about_to_show_menu);

  file_menu->addSeparator();
  AddSaveCurrentProjectAction(file_menu, *m_project_handler);
  AddSaveProjectAsAction(file_menu, *m_project_handler);

  file_menu->addSeparator();

  file_menu->addAction(m_import_action);
  file_menu->addAction(m_export_action);

  file_menu->addSeparator();
  auto preferences_menu = file_menu->addMenu("Preferences");
  preferences_menu->setToolTipsVisible(true);
  preferences_menu->addAction(m_system_font_action);
  preferences_menu->addSeparator();
  preferences_menu->addAction(m_reset_settings_action);

  file_menu->addSeparator();
  file_menu->addAction(m_exit_action);

  AppRegisterAction(constants::kHelpMenu, m_about_action);
}

void AnyValueEditorMainWindowActions::OnChangeSystemFont()
{
  if (SummonChangeSystemFontDialog())
  {
    emit RestartApplicationRequest(Restart);
  }
}

void AnyValueEditorMainWindowActions::OnResetSettings()
{
  if (ShouldResetSettingsAndRestart())
  {
    emit RestartApplicationRequest(CleanSettingsAndRestart);
  }
}

void AnyValueEditorMainWindowActions::OnAbout()
{
  AboutApplicationDialog::ShowDialog(mvvm::utils::FindMainWindow(), "AnyValue Editor",
                                     "AnyValue structure and value editor",
                                     QString::fromStdString(ProjectVersion()));
}

}  // namespace sup::gui
