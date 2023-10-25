/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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
#include <sup/gui/core/version.h>
#include <sup/gui/widgets/about_application_dialog.h>

#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QMainWindow>
#include <QMenuBar>

namespace anyvalueeditor
{

AnyValueEditorMainWindowActions::AnyValueEditorMainWindowActions(QMainWindow *mainwindow)
    : QObject(mainwindow)
{
  CreateActions(mainwindow);
  SetupMenus(mainwindow->menuBar());
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

void AnyValueEditorMainWindowActions::SetupMenus(QMenuBar *menubar)
{
  sup::gui::AppRegisterMenuBar(menubar);

  auto file_menu = sup::gui::AppAddMenu(sup::gui::constants::kFileMenu)->GetMenu();

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

  auto help_menu = sup::gui::AppAddMenu(sup::gui::constants::kHelpMenu)->GetMenu();
  help_menu->addAction(m_about_action);
}

void AnyValueEditorMainWindowActions::OnChangeSystemFont()
{
  if (sup::gui::SummonChangeSystemFontDialog())
  {
    emit RestartApplicationRequest(sup::gui::Restart);
  }
}

void AnyValueEditorMainWindowActions::OnResetSettings()
{
  if (sup::gui::ShouldResetSettingsAndRestart())
  {
    emit RestartApplicationRequest(sup::gui::CleanSettingsAndRestart);
  }
}

void AnyValueEditorMainWindowActions::OnAbout()
{
  sup::gui::AboutApplicationDialog::ShowDialog(mvvm::utils::FindMainWindow(), "AnyValue Editor",
                                               "AnyValue structure and value editor",
                                               QString::fromStdString(sup::gui::ProjectVersion()));
}

}  // namespace anyvalueeditor
