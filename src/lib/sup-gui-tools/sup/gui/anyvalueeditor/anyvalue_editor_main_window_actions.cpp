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
  m_import_action = new QAction("Open", this);
  m_import_action->setShortcuts(QKeySequence::Open);
  connect(m_import_action, &QAction::triggered, this,
          &AnyValueEditorMainWindowActions::OnImportFromFileRequest);

  m_export_action = new QAction("Save", this);
  m_export_action->setShortcuts(QKeySequence::Save);
  connect(m_export_action, &QAction::triggered, this,
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

  file_menu->addAction(m_import_action);
  file_menu->addAction(m_export_action);
  file_menu->addSeparator();
  file_menu->addAction(m_exit_action);

  auto help_menu = menubar->addMenu("&Help");
  help_menu->addAction(m_about_action);
}

void AnyValueEditorMainWindowActions::OnAbout()
{
  sup::gui::AboutApplicationDialog::ShowDialog(mvvm::utils::FindMainWindow(), "AnyValue Editor",
                                               "AnyValue structure and value editor",
                                               QString::fromStdString(sup::gui::ProjectVersion()));
}

}  // namespace anyvalueeditor
