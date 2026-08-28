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

#include "anyvalue_editor_main_window.h"

#include "anyvalue_editor_app_context.h"
#include "settings_helper.h"

#include <sup/gui/app/app_constants.h>
#include <sup/gui/components/anyvalue_editor_project.h>
#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/mainwindow/anyvalue_editor_main_window_actions.h>
#include <sup/gui/mainwindow/main_window_helper.h>
#include <sup/gui/model/persistent_settings_model.h>
#include <sup/gui/model/settings_constants.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_widget.h>

#include <mvvm/model/application_model.h>
#include <mvvm/widgets/app_restarter.h>

#include <QCloseEvent>
#include <QCoreApplication>
#include <QMenuBar>
#include <QSettings>

namespace sup::gui
{

namespace
{
const QString kMainWindowGroupName("MainWindow");

QString GetWindowSizeSettingName()
{
  return kMainWindowGroupName + "/" + "size";
}

QString GetWindowPosSettingName()
{
  return kMainWindowGroupName + "/" + "position";
}

}  // namespace

AnyValueEditorMainWindow::AnyValueEditorMainWindow(AnyValueEditorAppContext& context)
    : m_context(context)
    , m_settings(std::make_unique<PersistentSettingsModel>())
    , m_project(CreateProject())
{
  InitApplication();

  m_project->CreateEmpty();
}

AnyValueEditorMainWindow::~AnyValueEditorMainWindow() = default;

void AnyValueEditorMainWindow::ImportAnyValueFromFile(const QString& file_name)
{
  m_anyvalue_editor->ImportAnyValueFromFile(file_name);
}

void AnyValueEditorMainWindow::closeEvent(QCloseEvent* event)
{
  if (CanCloseApplication())
  {
    QMainWindow::closeEvent(event);
    return;
  }
  event->ignore();
}

void AnyValueEditorMainWindow::InitApplication()
{
  ReadSettings();
  InitComponents();
}

void AnyValueEditorMainWindow::InitComponents()
{
  // it should be initilised first, since it creates global proxy actions used by others
  m_action_manager = new AnyValueEditorMainWindowActions(m_project.get(), m_settings.get(),
                                                         m_context.GetCommandService(), this);

  m_anyvalue_editor = new AnyValueEditorWidget(m_context.GetCommandService());

  setCentralWidget(m_anyvalue_editor);

  connect(m_action_manager, &AnyValueEditorMainWindowActions::RestartApplicationRequest, this,
          &AnyValueEditorMainWindow::OnExitRequest);
  connect(m_action_manager, &AnyValueEditorMainWindowActions::OnImportFromFileRequest,
          m_anyvalue_editor, &AnyValueEditorWidget::OnImportFromFileRequest);
  connect(m_action_manager, &AnyValueEditorMainWindowActions::OnExportToFileRequest,
          m_anyvalue_editor, &AnyValueEditorWidget::OnExportToFileRequest);
  connect(m_action_manager, &AnyValueEditorMainWindowActions::ProjectLoaded, this,
          &AnyValueEditorMainWindow::OnProjectLoad);

  connect(m_action_manager, &AnyValueEditorMainWindowActions::OnImportWaveformRequest,
          m_anyvalue_editor, &AnyValueEditorWidget::OnImportWaveformRequest);
}

void AnyValueEditorMainWindow::ReadSettings()
{
  const QSettings settings;
  resize(settings.value(GetWindowSizeSettingName(), QSize(800, 600)).toSize());
  move(settings.value(GetWindowPosSettingName(), QPoint(200, 200)).toPoint());
  ReadApplicationSettings(*m_settings);
}

void AnyValueEditorMainWindow::WriteSettings()
{
  QSettings settings;
  settings.setValue(GetWindowSizeSettingName(), size());
  settings.setValue(GetWindowPosSettingName(), pos());
  WriteApplicationSettings(*m_settings);
}

bool AnyValueEditorMainWindow::CanCloseApplication()
{
  if (m_action_manager->CloseCurrentProject())
  {
    WriteSettings();
    return true;
  }

  return false;
}

void AnyValueEditorMainWindow::OnExitRequest(mvvm::AppExitRequest exit_request)
{
  if (CanCloseApplication())
  {
    mvvm::AppRestarter::StoreExitRequest(exit_request);
    close();
  }
}

void AnyValueEditorMainWindow::OnProjectLoad()
{
  if (m_project->GetApplicationModel() == nullptr)
  {
    throw RuntimeException("No model exists");
  }

  const auto enable_undo = m_settings->Data<bool>(constants::kUseUndoSetting);
  const auto undo_limit = m_settings->Data<int>(constants::kUndoLimitSetting);
  m_project->GetApplicationModel()->SetUndoEnabled(enable_undo, undo_limit);

  m_anyvalue_editor->SetAnyValueItemContainer(m_project->GetApplicationModel()->GetRootItem());
  UpdateProjectNames();
}

void AnyValueEditorMainWindow::UpdateProjectNames()
{
  m_action_manager->UpdateProjectNames();
}

std::unique_ptr<AnyValueEditorProject> AnyValueEditorMainWindow::CreateProject()
{
  mvvm::ProjectContext context;
  context.modified_callback = [this]() { UpdateProjectNames(); };
  context.loaded_callback = [this]() { OnProjectLoad(); };
  context.saved_callback = [this]() { UpdateProjectNames(); };
  context.application_type = std::string(constants::kAnyValueEditorApplicationType);
  return std::make_unique<AnyValueEditorProject>(context);
}

}  // namespace sup::gui
