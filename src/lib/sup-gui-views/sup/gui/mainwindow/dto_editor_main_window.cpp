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

#include "dto_editor_main_window_actions.h"

#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/app/application_helper.h>
#include <sup/gui/views/dtoeditor/dto_composer_view.h>
#include <sup/gui/widgets/style_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/container_item.h>
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

DtoEditorMainWindow::DtoEditorMainWindow() : m_model(std::make_unique<mvvm::ApplicationModel>())
{
  PopulateModel();
  InitApplication();
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

void DtoEditorMainWindow::PopulateModel()
{
  // Adding empty container for first AnyValueItem. Item itself will be created from
  // SupDtoComposerView.
  m_model->InsertItem<mvvm::ContainerItem>();
}

void DtoEditorMainWindow::InitApplication()
{
  ReadSettings();
  InitComponents();
}

void DtoEditorMainWindow::InitComponents()
{
  m_action_manager = new DtoEditorMainWindowActions(m_model.get(), this);

  m_tab_widget = new mvvm::MainVerticalBarWidget;
  m_tab_widget->SetBaseColor("#008a65");

  m_composer_view = new DtoComposerView(m_model.get());
  m_tab_widget->AddWidget(m_composer_view, "Compose",
                          utils::GetIcon("file-tree-outline-light.svg"));
  m_tab_widget->AddWidget(new QWidget, "Compare",
                          utils::GetIcon("application-brackets-outline-light.svg"));
  m_tab_widget->AddWidget(new QWidget, "Bulk Edit", utils::GetIcon("animation-outline-light.svg"));

  m_tab_widget->AddSpacer();
  m_tab_widget->SetCurrentIndex(0);

  setCentralWidget(m_tab_widget);

  connect(m_action_manager, &DtoEditorMainWindowActions::RestartApplicationRequest, this,
          &DtoEditorMainWindow::OnRestartRequest);
}

void DtoEditorMainWindow::ReadSettings()
{
  const QSettings settings;
  const auto default_size = QSize(mvvm::utils::UnitSize(80), mvvm::utils::UnitSize(60));
  resize(settings.value(kWindowSizeSettingName, default_size).toSize());

  const auto default_pos = QPoint(mvvm::utils::UnitSize(20), mvvm::utils::UnitSize(40));
  move(settings.value(kWindowPosSettingName, default_pos).toPoint());
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

}  // namespace sup::gui
