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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "dto_editor_main_window.h"

#include "color_palette.h"
#include "dto_editor_main_window_actions.h"
#include "settings_helper.h"

#include <sup/gui/app/app_constants.h>
#include <sup/gui/mainwindow/main_window_helper.h>
#include <sup/gui/model/settings_constants.h>
#include <sup/gui/model/settings_model.h>
#include <sup/gui/model/sup_dto_model.h>
#include <sup/gui/model/waveform_model.h>
#include <sup/gui/style/style_helper.h>
#include <sup/gui/views/dtoeditor/dto_composer_view.h>
#include <sup/gui/views/dtoeditor/dto_waveform_view.h>

#include <mvvm/widgets/main_vertical_bar_widget.h>
#include <mvvm/widgets/widget_utils.h>

#include <QCloseEvent>
#include <QCoreApplication>
#include <QMenuBar>
#include <QSettings>

namespace
{
const QString kGroupName = "MainWindow";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";
const QString kWindowPosSettingName = kGroupName + "/" + "position";
}  // namespace

namespace sup::gui
{

DtoEditorMainWindow::DtoEditorMainWindow() : m_project(CreateProject())
{
  InitApplication();

  m_project->CreateEmpty();
}

DtoEditorMainWindow::~DtoEditorMainWindow() = default;

void DtoEditorMainWindow::closeEvent(QCloseEvent* event)
{
  if (CanCloseApplication())
  {
    ShutdownApplication();
    QMainWindow::closeEvent(event);
    return;
  }
  event->ignore();
}

void DtoEditorMainWindow::InitApplication()
{
  ReadSettings();
  InitComponents();
}

void DtoEditorMainWindow::InitComponents()
{
  m_action_manager = new DtoEditorMainWindowActions(m_project.get(), this);

  m_tab_widget = new mvvm::MainVerticalBarWidget;

  m_tab_widget->SetBaseColor(GetMainToolBarColor());

  m_composer_view = new DtoComposerView;
  m_tab_widget->AddWidget(m_composer_view, "Compose",
                          utils::FindIcon("file-tree-outline", IconColorFlavor::kForDarkThemes));

  m_waveform_view = new DtoWaveformView;
  m_tab_widget->AddWidget(
      m_waveform_view, "Waveforms",
      utils::FindIcon("chart-timeline-variant-shimmer", IconColorFlavor::kForDarkThemes));

  m_tab_widget->AddWidget(
      new QWidget, "Compare",
      utils::FindIcon("application-brackets-outline", IconColorFlavor::kForDarkThemes));

  m_tab_widget->AddWidget(new QWidget, "Bulk Edit",
                          utils::FindIcon("animation-outline", IconColorFlavor::kForDarkThemes));

  m_tab_widget->AddSpacer();
  m_tab_widget->SetCurrentIndex(0);

  setCentralWidget(m_tab_widget);

  connect(m_action_manager, &DtoEditorMainWindowActions::RestartApplicationRequest, this,
          &DtoEditorMainWindow::OnRestartRequest);
  connect(m_action_manager, &DtoEditorMainWindowActions::ProjectLoaded, this,
          &DtoEditorMainWindow::OnProjectLoad);

  m_action_manager->SetupStatusBar(m_tab_widget->GetStatusBar());
}

void DtoEditorMainWindow::ReadSettings()
{
  const QSettings settings;
  const auto default_size = QSize(mvvm::utils::UnitSize(80), mvvm::utils::UnitSize(60));
  resize(settings.value(kWindowSizeSettingName, default_size).toSize());

  const auto default_pos = QPoint(mvvm::utils::UnitSize(20), mvvm::utils::UnitSize(40));
  move(settings.value(kWindowPosSettingName, default_pos).toPoint());
  // global persistent setting stored in SettingsModel
  sup::gui::ReadGlobalSettings();
}

void DtoEditorMainWindow::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWindowSizeSettingName, size());
  settings.setValue(kWindowPosSettingName, pos());
}

bool DtoEditorMainWindow::CanCloseApplication()
{
  WriteSettings();

  if (m_action_manager->CloseCurrentProject())
  {
    WriteSettings();
    return true;
  }

  return false;
}

void DtoEditorMainWindow::OnRestartRequest(sup::gui::AppExitCode exit_code)
{
  if (CanCloseApplication())
  {
    ShutdownApplication();
    QCoreApplication::exit(exit_code);
  }
}

void DtoEditorMainWindow::OnProjectLoad()
{
  const auto enable_undo =
      sup::gui::GetGlobalSettings().Data<bool>(sup::gui::constants::kUseUndoSetting);
  const auto undo_limit =
      sup::gui::GetGlobalSettings().Data<int>(sup::gui::constants::kUndoLimitSetting);
  m_project->GetWaveformModel()->SetUndoEnabled(enable_undo, undo_limit);
  m_project->GetSupDtoModel()->SetUndoEnabled(enable_undo, undo_limit);

  m_composer_view->SetModel(m_project->GetSupDtoModel());
  m_waveform_view->SetWaveformModel(m_project->GetWaveformModel());
  UpdateProjectNames();
}

void DtoEditorMainWindow::UpdateProjectNames()
{
  m_action_manager->UpdateProjectNames();
}

std::unique_ptr<DtoEditorProject> DtoEditorMainWindow::CreateProject()
{
  mvvm::ProjectContext context;
  context.modified_callback = [this]() { UpdateProjectNames(); };
  context.loaded_callback = [this]() { OnProjectLoad(); };
  context.saved_callback = [this]() { UpdateProjectNames(); };
  context.application_type = constants::kDtoEditorApplicationType.toStdString();
  return std::make_unique<DtoEditorProject>(context);
}

}  // namespace sup::gui
