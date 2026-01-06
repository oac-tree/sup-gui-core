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

#include "anyvalue_editor_main_window_actions.h"

#include "about_application_dialog.h"

#include <sup/gui/app/app_command.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/app_context_focus_controller.h>
#include <sup/gui/app/i_app_command_service.h>
#include <sup/gui/app/menu_bar_action_registry.h>
#include <sup/gui/component_objects/proxy_action.h>
#include <sup/gui/core/version.h>
#include <sup/gui/mainwindow/main_window_helper.h>
#include <sup/gui/mainwindow/settings_editor_dialog.h>
#include <sup/gui/mainwindow/settings_helper.h>
#include <sup/gui/mainwindow/status_bar_helper.h>
#include <sup/gui/style/style_helper.h>

#include <mvvm/project/project_handler.h>
#include <mvvm/project/project_handler_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolButton>

namespace sup::gui
{

AnyValueEditorMainWindowActions::AnyValueEditorMainWindowActions(
    mvvm::IProject* project, mvvm::ISessionModel* settings, IAppCommandService& command_service,
    QMainWindow* main_window)
    : QObject(main_window)
    , m_settings(settings)
    , m_command_service(command_service)
    , m_project_handler(std::make_unique<mvvm::ProjectHandler>(project))
{
  m_command_service.AppRegisterMenuBar(
      main_window->menuBar(),
      {constants::kFileMenu, constants::kEditMenu, constants::kViewMenu, constants::kHelpMenu});

  CreateActions(main_window);
  SetupMenus();
  SetupStatusBar(main_window->statusBar());

  (void)QObject::connect(qApp, &QApplication::focusChanged, this, [this](QWidget* old, QWidget* now)
                         { m_command_service.OnFocusWidgetUpdate(old, now); });
}

AnyValueEditorMainWindowActions::~AnyValueEditorMainWindowActions() = default;

bool AnyValueEditorMainWindowActions::CloseCurrentProject() const
{
  return m_project_handler->CloseProject();
}

void AnyValueEditorMainWindowActions::UpdateProjectNames()
{
  m_project_handler->UpdateNames();
}

void AnyValueEditorMainWindowActions::SetupStatusBar(QStatusBar* status_bar)
{
  m_toggle_right_sidebar_button = new QToolButton;
  m_toggle_right_sidebar_button->setToolTip("Show/hide right panel");
  m_toggle_right_sidebar_button->setIcon(utils::FindIcon("dock-right"));
  auto action =
      m_command_service.GetCommand(constants::kToggleRightPanelCommandId)->GetProxyAction();
  SetupStatusBarButton(m_toggle_right_sidebar_button, action);

  status_bar->addPermanentWidget(m_toggle_right_sidebar_button, 0);
}

void AnyValueEditorMainWindowActions::CreateActions(QMainWindow* mainwindow)
{
  m_import_action = new QAction("Import", this);
  m_import_action->setShortcuts(QKeySequence::Open);
  connect(m_import_action, &QAction::triggered, this,
          &AnyValueEditorMainWindowActions::OnImportFromFileRequest);

  m_import_waveform_action = new QAction("Create Waveform", this);
  connect(m_import_waveform_action, &QAction::triggered, this,
          &AnyValueEditorMainWindowActions::OnImportWaveformRequest);

  m_export_action = new QAction("Export", this);
  connect(m_export_action, &QAction::triggered, this,
          &AnyValueEditorMainWindowActions::OnExportToFileRequest);

  m_system_font_action = new QAction("System font (restart required)", this);
  m_system_font_action->setStatusTip("Summon font settings dialog");
  connect(m_system_font_action, &QAction::triggered, this,
          &AnyValueEditorMainWindowActions::OnChangeSystemFont);

  m_settings_dialog_action = new QAction("Application settings", this);
  m_settings_dialog_action->setStatusTip("Summon application settings dialog");
  connect(m_settings_dialog_action, &QAction::triggered, this,
          &AnyValueEditorMainWindowActions::OnApplicationSettingsDialog);

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
  SetupFileMenu();
  SetupEditMenu();
  SetupViewMenu();
  SetupHelpMenu();
}

void AnyValueEditorMainWindowActions::SetupFileMenu()
{
  auto file_menu = m_command_service.AppGetMenu(constants::kFileMenu);

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
  preferences_menu->addAction(m_settings_dialog_action);
  preferences_menu->addSeparator();
  preferences_menu->addAction(m_reset_settings_action);

  file_menu->addSeparator();
  file_menu->addAction(m_exit_action);
}

void AnyValueEditorMainWindowActions::SetupEditMenu()
{
  auto command =
      m_command_service.AddCommandToMenu(constants::kEditMenu, constants::kUndoCommandId);
  command->SetText("Undo").SetShortcut(QKeySequence::Undo);

  command = m_command_service.AddCommandToMenu(constants::kEditMenu, constants::kRedoCommandId);
  command->SetText("Redo").SetShortcut(QKeySequence::Redo);

  m_command_service.AppGetMenu(constants::kEditMenu)->addSeparator();

  command = m_command_service.AddCommandToMenu(constants::kEditMenu, constants::kCutCommandId);
  command->SetText("Cut").SetShortcut(QKeySequence::Cut);

  command = m_command_service.AddCommandToMenu(constants::kEditMenu, constants::kCopyCommandId);
  command->SetText("Copy").SetShortcut(QKeySequence::Copy);

  command = m_command_service.AddCommandToMenu(constants::kEditMenu, constants::kPasteCommandId);
  command->SetText("Paste").SetShortcut(QKeySequence::Paste);

  command =
      m_command_service.AddCommandToMenu(constants::kEditMenu, constants::kPasteSpecialCommandId);
  command->SetText("Paste Special").SetShortcut(QKeySequence("Ctrl+Shift+V"));

  command =
      m_command_service.AddCommandToMenu(constants::kEditMenu, constants::kRemoveSelectedCommandId);
  command->SetText("Remove selected");
  command->GetProxyAction()->setShortcuts({Qt::Key_Delete, Qt::Key_Backspace});

  auto menu = m_command_service.AppGetMenu(constants::kEditMenu);
  menu->addSeparator();
  menu->addAction(m_import_waveform_action);
}

void AnyValueEditorMainWindowActions::SetupViewMenu()
{
  auto command = m_command_service.AddCommandToMenu(constants::kViewMenu,
                                                    constants::kToggleRightPanelCommandId);
  command->SetShortcut(QKeySequence("Alt+Shift+0"));
}

void AnyValueEditorMainWindowActions::SetupHelpMenu()
{
  m_command_service.AddActionToMenuBar(constants::kHelpMenu, m_about_action);
}

void AnyValueEditorMainWindowActions::OnChangeSystemFont()
{
  if (SummonChangeSystemFontDialog())
  {
    emit RestartApplicationRequest(Restart);
  }
}

void AnyValueEditorMainWindowActions::OnApplicationSettingsDialog()
{
  if (m_settings == nullptr)
  {
    return;
  }
  SettingsEditorDialog dialog(*m_settings);
  if (dialog.exec() == QDialog::Accepted)
  {
    dialog.PropagateSettingsToModel(*m_settings);
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
