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

#include <sup/gui/anyvalueeditor/anyvalue_editor.h>
#include <sup/gui/anyvalueeditor/anyvalue_editor_main_window_actions.h>

#include <mvvm/model/application_model.h>

#include <QCloseEvent>
#include <QCoreApplication>
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

namespace anyvalueeditor
{
AnyValueEditorMainWindow::AnyValueEditorMainWindow()
{
  InitApplication();
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
  m_anyvalue_editor = new sup::gui::AnyValueEditor;

  m_action_manager = new AnyValueEditorMainWindowActions(m_anyvalue_editor->GetModel(), this);
  connect(m_action_manager, &AnyValueEditorMainWindowActions::RestartApplicationRequest, this,
          &AnyValueEditorMainWindow::OnRestartRequest);

  setCentralWidget(m_anyvalue_editor);

  connect(m_action_manager, &AnyValueEditorMainWindowActions::OnImportFromFileRequest,
          m_anyvalue_editor, &sup::gui::AnyValueEditor::OnImportFromFileRequest);
  connect(m_action_manager, &AnyValueEditorMainWindowActions::OnExportToFileRequest,
          m_anyvalue_editor, &sup::gui::AnyValueEditor::OnExportToFileRequest);
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

  // here will be logic for unsaved project
  return true;
}

void AnyValueEditorMainWindow::OnRestartRequest(sup::gui::AppExitCode exit_code)
{
  if (CanCloseApplication())
  {
    QCoreApplication::exit(exit_code);
  }
}

}  // namespace anyvalueeditor
