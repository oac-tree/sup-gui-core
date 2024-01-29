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

#include "dto_editor_main_window.h"

#include <sup/gui/app/app_action_helper.h>

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

namespace dtoeditor
{

DtoEditorMainWindow::DtoEditorMainWindow()
{
  InitApplication();
}

DtoEditorMainWindow::~DtoEditorMainWindow() = default;

void DtoEditorMainWindow::closeEvent(QCloseEvent* event)
{
  if (PrepareForShutdown())
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
  sup::gui::AppAddMenus(menuBar(), {sup::gui::constants::kFileMenu, sup::gui::constants::kViewMenu,
                                    sup::gui::constants::kHelpMenu});

  setCentralWidget(new QWidget);
}

void DtoEditorMainWindow::ReadSettings() {}

void DtoEditorMainWindow::WriteSettings() {}

bool DtoEditorMainWindow::PrepareForShutdown()
{
  WriteSettings();
  return true;
}

void DtoEditorMainWindow::OnRestartRequest(sup::gui::AppExitCode exit_code)
{
  if (PrepareForShutdown())
  {
    QCoreApplication::exit(exit_code);
  }
}

}  // namespace dtoeditor
