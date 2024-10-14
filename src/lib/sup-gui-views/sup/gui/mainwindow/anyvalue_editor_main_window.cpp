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

#include "anyvalue_editor_main_window.h"

#include <sup/gui/mainwindow/anyvalue_editor_main_window_actions.h>
#include <sup/gui/mainwindow/main_window_helper.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_widget.h>

#include <mvvm/model/application_model.h>

#include <QCloseEvent>
#include <QCoreApplication>
#include <QDialog>
#include <QMenuBar>
#include <QSettings>

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

namespace sup::gui
{
AnyValueEditorMainWindow::AnyValueEditorMainWindow()
    : m_model(std::make_unique<mvvm::ApplicationModel>())
{
  InitApplication();
  OnProjectLoad();
}

AnyValueEditorMainWindow::~AnyValueEditorMainWindow() = default;

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
  // it should be initialised first, since it creates global proxy actions used by others
  m_action_manager = new AnyValueEditorMainWindowActions(this);

  m_anyvalue_editor = new sup::gui::AnyValueEditorWidget;
  m_action_manager->SetModel(m_model.get());

  setCentralWidget(m_anyvalue_editor);

  connect(m_action_manager, &AnyValueEditorMainWindowActions::RestartApplicationRequest, this,
          &AnyValueEditorMainWindow::OnRestartRequest);
  connect(m_action_manager, &AnyValueEditorMainWindowActions::OnImportFromFileRequest,
          m_anyvalue_editor, &sup::gui::AnyValueEditorWidget::OnImportFromFileRequest);
  connect(m_action_manager, &AnyValueEditorMainWindowActions::OnExportToFileRequest,
          m_anyvalue_editor, &sup::gui::AnyValueEditorWidget::OnExportToFileRequest);
  connect(m_action_manager, &AnyValueEditorMainWindowActions::ProjectLoaded, this,
          &AnyValueEditorMainWindow::OnProjectLoad);

  connect(m_action_manager, &AnyValueEditorMainWindowActions::OnImportWaveformRequest,
          m_anyvalue_editor, &sup::gui::AnyValueEditorWidget::OnImportWaveformRequest);
}

void AnyValueEditorMainWindow::ReadSettings()
{
  const QSettings settings;
  resize(settings.value(GetWindowSizeSettingName(), QSize(800, 600)).toSize());
  move(settings.value(GetWindowPosSettingName(), QPoint(200, 200)).toPoint());
}

void AnyValueEditorMainWindow::WriteSettings()
{
  QSettings settings;
  settings.setValue(GetWindowSizeSettingName(), size());
  settings.setValue(GetWindowPosSettingName(), pos());
}

bool AnyValueEditorMainWindow::CanCloseApplication()
{
  WriteSettings();

  if (m_action_manager->CloseCurrentProject())
  {
    WriteSettings();
    return true;
  }

  return false;
}

void AnyValueEditorMainWindow::OnRestartRequest(sup::gui::AppExitCode exit_code)
{
  if (CanCloseApplication())
  {
    ShutdownApplication();
    QCoreApplication::exit(exit_code);
  }
}

void AnyValueEditorMainWindow::OnProjectLoad()
{
  m_anyvalue_editor->SetAnyValueItemContainer(m_model->GetRootItem());
}

}  // namespace sup::gui
