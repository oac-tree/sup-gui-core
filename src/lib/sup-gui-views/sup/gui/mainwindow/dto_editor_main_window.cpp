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

#include "dto_editor_main_window.h"

#include "anyvalue_editor_app_context.h"
#include "color_palette.h"
#include "dto_editor_main_window_actions.h"

#include <sup/gui/app/app_constants.h>
#include <sup/gui/mainwindow/main_window_helper.h>
#include <sup/gui/model/sup_dto_model.h>
#include <sup/gui/model/undo_settings_item.h>
#include <sup/gui/model/waveform_model.h>
#include <sup/gui/settings/i_settings_manager.h>
#include <sup/gui/style/style_helper.h>
#include <sup/gui/views/dtoeditor/dto_compare_view.h>
#include <sup/gui/views/dtoeditor/dto_composer_view.h>
#include <sup/gui/views/dtoeditor/dto_explorer_view.h>
#include <sup/gui/views/dtoeditor/dto_waveform_view.h>
#include <sup/gui/widgets/window_info.h>

#include <mvvm/widgets/app_restarter.h>
#include <mvvm/widgets/main_vertical_bar_widget.h>

#include <QCloseEvent>
#include <QDebug>
#include <QMenuBar>

namespace
{
//! Settings group of main window geometry.
constexpr auto kWindowSettingsGroup = "MainWindow";
}  // namespace

namespace sup::gui
{

DtoEditorMainWindow::DtoEditorMainWindow(AnyValueEditorAppContext& context)
    : m_context(context), m_project(CreateProject())
{
  InitApplication();

  m_project->CreateEmpty();
}

DtoEditorMainWindow::~DtoEditorMainWindow() = default;

void DtoEditorMainWindow::ImportAnyValueFromFile(const QString& file_name)
{
  (void)file_name;
  qDebug() << "Import is not implemented yet";
}

void DtoEditorMainWindow::closeEvent(QCloseEvent* event)
{
  if (CanCloseApplication())
  {
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
  m_action_manager = new DtoEditorMainWindowActions(m_project.get(), m_context.GetSettingsManager(),
                                                    m_context.GetCommandService(), this);

  m_tab_widget = new mvvm::MainVerticalBarWidget;

  m_tab_widget->SetBaseColor(GetMainToolBarColor());

  // explorer view
  m_explorer_view = new DtoExplorerView(m_context.GetSettingsManager());
  m_tab_widget->AddWidget(
      m_explorer_view, "Explore",
      utils::FindIcon("file-search-outline", mvvm::ColorFlavor::kForDarkThemes));

  // compare view
  m_compare_view = new DtoCompareView(m_context.GetSettingsManager());
  m_tab_widget->AddWidget(
      m_compare_view, "Compare",
      utils::FindIcon("application-brackets-outline", mvvm::ColorFlavor::kForDarkThemes));

  // composer view
  m_composer_view = new DtoComposerView(m_context.GetViewContext());
  m_tab_widget->AddWidget(m_composer_view, "Compose",
                          utils::FindIcon("file-tree-outline", mvvm::ColorFlavor::kForDarkThemes));

  // waveform view
  m_waveform_view = new DtoWaveformView(m_context.GetViewContext());
  m_tab_widget->AddWidget(
      m_waveform_view, "Waveforms",
      utils::FindIcon("chart-timeline-variant-shimmer", mvvm::ColorFlavor::kForDarkThemes));

  // bulk edit
  m_tab_widget->AddWidget(new QWidget, "Bulk Edit",
                          utils::FindIcon("animation-outline", mvvm::ColorFlavor::kForDarkThemes));

  m_tab_widget->AddSpacer();
  m_tab_widget->SetCurrentIndex(0);

  setCentralWidget(m_tab_widget);

  connect(m_action_manager, &DtoEditorMainWindowActions::ExitRequest, this,
          &DtoEditorMainWindow::OnExitRequest);
  connect(m_action_manager, &DtoEditorMainWindowActions::ProjectLoaded, this,
          &DtoEditorMainWindow::OnProjectLoad);

  m_action_manager->SetupStatusBar(m_tab_widget->GetStatusBar());
}

void DtoEditorMainWindow::ReadSettings()
{
  auto& settings_manager = m_context.GetSettingsManager();
  const auto window_info =
      settings_manager.Get<WindowInfo>(kWindowSettingsGroup, SettingsScope::kApplication);
  if (!restoreGeometry(window_info.geometry))
  {
    resize(QSize(800, 600));
  }
  m_window_settings_connection =
      settings_manager.RegisterProvider(kWindowSettingsGroup, SettingsScope::kApplication,
                                        [this]() { return WindowInfo{saveGeometry()}; });
}

void DtoEditorMainWindow::WriteSettings()
{
  m_context.GetSettingsManager().Collect(SettingsScope::kApplication);
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

void DtoEditorMainWindow::OnExitRequest(mvvm::AppExitRequest exit_request)
{
  if (CanCloseApplication())
  {
    mvvm::AppRestarter::StoreExitRequest(exit_request);
    close();
  }
}

void DtoEditorMainWindow::OnProjectLoad()
{
  const auto undo_info =
      m_context.GetSettingsManager().Get<UndoInfo>(kUndoSettingsGroup, SettingsScope::kApplication);
  m_project->GetWaveformModel()->SetUndoEnabled(undo_info.is_enabled, undo_info.undo_limit);
  m_project->GetSupDtoModel()->SetUndoEnabled(undo_info.is_enabled, undo_info.undo_limit);

  m_explorer_view->SetConfigurationModel(m_project->GetConfigurationModel());
  m_compare_view->SetConfigurationModel(m_project->GetConfigurationModel());
  m_composer_view->SetConfigurationModel(m_project->GetConfigurationModel());
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
  context.application_type = std::string(constants::kDtoEditorApplicationType);
  return std::make_unique<DtoEditorProject>(context);
}

}  // namespace sup::gui
