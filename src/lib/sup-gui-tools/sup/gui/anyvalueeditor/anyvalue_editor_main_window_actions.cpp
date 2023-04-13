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

#include "about_application_dialog.h"

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
  m_open_action = new QAction("Open", this);
  m_open_action->setShortcuts(QKeySequence::Open);
  connect(m_open_action, &QAction::triggered, this,
          &AnyValueEditorMainWindowActions::OnImportFromFileRequest);

  m_save_action = new QAction("Save", this);
  m_save_action->setShortcuts(QKeySequence::Save);
  connect(m_save_action, &QAction::triggered, this,
          &AnyValueEditorMainWindowActions::OnExportToFileRequest);

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
  auto file_menu = menubar->addMenu("&File");

  file_menu->addAction(m_open_action);
  file_menu->addAction(m_save_action);
  file_menu->addSeparator();
  file_menu->addAction(m_exit_action);

  auto help_menu = menubar->addMenu("&Help");
  help_menu->addAction(m_about_action);
}

void AnyValueEditorMainWindowActions::OnAbout()
{
  AboutApplicationDialog dialog(mvvm::utils::FindMainWindow());
  dialog.exec();
}

}  // namespace anyvalueeditor
