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

#include "anyvalue_editor.h"

#include "anyvalue_editor_actions.h"
#include "anyvalue_editor_textpanel.h"
#include "anyvalue_editor_toolbar.h"
#include "anyvalue_editor_treepanel.h"

#include <mvvm/model/application_model.h>
#include <mvvm/project/model_has_changed_controller.h>
#include <mvvm/utils/file_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSettings>
#include <QSplitter>

namespace
{
const QString kGroupName("AnyValueEditor");

QString GetCurrentWorkdirSettingName()
{
  return kGroupName + "/" + "workdir";
}

QString GetSplitterSettingName()
{
  return kGroupName + "/" + "splitter";
}

QString GetIsVisiblePanelSettingName()
{
  return kGroupName + "/" + "json_panel";
}

}  // namespace

namespace sup::gui
{

AnyValueEditor::AnyValueEditor(QWidget *parent)
    : QWidget(parent)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_actions(new AnyValueEditorActions(CreateActionContext(), m_model.get(), this))
    , m_tool_bar(new AnyValueEditorToolBar(m_actions))
    , m_text_edit(new AnyValueEditorTextPanel(m_model.get()))
    , m_tree_panel(new AnyValueEditorTreePanel(m_model.get()))
    , m_splitter(new QSplitter)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);

  m_splitter->addWidget(m_tree_panel);
  m_splitter->addWidget(m_text_edit);
  m_splitter->setCollapsible(0, false);
  m_splitter->setCollapsible(1, false);

  layout->addWidget(m_splitter);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_model->RegisterItem<sup::gui::AnyValueStructItem>();
  m_model->RegisterItem<sup::gui::AnyValueArrayItem>();
  m_model->RegisterItem<sup::gui::AnyValueScalarItem>();

  SetupConnections();

  ReadSettings();
}

AnyValueEditor::~AnyValueEditor()
{
  WriteSettings();
}

//! Imports AnyValue from JSON file.

void AnyValueEditor::OnImportFromFileRequest()
{
  QFileDialog dialog(this, "Select JSON file to load", m_current_workdir);
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setNameFilter("All JSON files (*.json *.JSON)");
  //  dialog.setOption(QFileDialog::DontUseNativeDialog);
  QStringList selected_files = dialog.exec() ? dialog.selectedFiles() : QStringList();
  auto file_name = selected_files.empty() ? QString() : selected_files.at(0);

  if (!file_name.isEmpty())
  {
    ImportAnyValueFromFile(file_name);
    UpdateCurrentWorkdir(file_name);
  }
}

void AnyValueEditor::OnExportToFileRequest()
{
  auto file_name = QFileDialog::getSaveFileName(
      this, "Save File", m_current_workdir + "/untitled.json", tr("Images (*.json *.JSON)"));

  if (!file_name.isEmpty())
  {
    m_actions->OnExportToFileRequest(file_name.toStdString());
    UpdateCurrentWorkdir(file_name);
  }
}

//! Returns AnyValueItem selected by the user in item tree.

sup::gui::AnyValueItem *AnyValueEditor::GetSelectedItem() const
{
  return m_tree_panel->GetSelectedItem();
}

//! Sets initial value. The given value will be cloned inside the editor's model and used as
//! a starting point for editing.

void AnyValueEditor::SetInitialValue(const AnyValueItem &item)
{
  m_actions->SetInitialValue(item);
}

AnyValueItem *AnyValueEditor::GetTopItem()
{
  return m_actions->GetTopItem();
}

void AnyValueEditor::ReadSettings()
{
  const QSettings settings;
  m_current_workdir = settings.value(GetCurrentWorkdirSettingName(), QDir::homePath()).toString();

  if (settings.contains(GetSplitterSettingName()))
  {
    m_splitter->restoreState(settings.value(GetSplitterSettingName()).toByteArray());
  }

  m_text_panel_is_visible = settings.value(GetIsVisiblePanelSettingName(), true).toBool();
  m_text_edit->setVisible(m_text_panel_is_visible);
}

void AnyValueEditor::WriteSettings()
{
  QSettings settings;
  settings.setValue(GetCurrentWorkdirSettingName(), m_current_workdir);
  settings.setValue(GetSplitterSettingName(), m_splitter->saveState());
  settings.setValue(GetIsVisiblePanelSettingName(), m_text_panel_is_visible);
}

//! Set up all connections.

void AnyValueEditor::SetupConnections()
{
  auto on_panel = [this]()
  {
    m_text_panel_is_visible = !m_text_panel_is_visible;
    m_text_edit->setVisible(m_text_panel_is_visible);
  };
  connect(m_tool_bar, &AnyValueEditorToolBar::ToggleTextPanelVisibilityRequest, this, on_panel);

  connect(m_tool_bar, &AnyValueEditorToolBar::ImportFromFileRequest, this,
          &AnyValueEditor::OnImportFromFileRequest);

  connect(m_tool_bar, &AnyValueEditorToolBar::ExportToFileRequest, this,
          &AnyValueEditor::OnExportToFileRequest);
}

//! Creates a context with all callbacks necessary for AnyValueEditorActions to function.

AnyValueEditorContext AnyValueEditor::CreateActionContext() const
{
  auto get_selected_callback = [this]() { return GetSelectedItem(); };

  auto notify_warning_callback = [this](const sup::gui::MessageEvent &event)
  {
    QMessageBox msg_box;
    msg_box.setText(QString::fromStdString(event.text));
    msg_box.setInformativeText(QString::fromStdString(event.informative));
    msg_box.setDetailedText(QString::fromStdString(event.detailed));
    msg_box.setIcon(msg_box.Warning);
    msg_box.exec();
  };

  return {get_selected_callback, notify_warning_callback};
}

//! Updates cached value for last working directory for later saving in widget's persistent settings
void AnyValueEditor::UpdateCurrentWorkdir(const QString &file_name)
{
  auto parent_path = mvvm::utils::GetParentPath(file_name.toStdString());
  m_current_workdir = QString::fromStdString(parent_path);
}

//! Imports AnyValue from JSON file.

void AnyValueEditor::ImportAnyValueFromFile(const QString &file_name)
{
  // FIXME expandAll will not work if uncomment. Check this after the refactoring of
  // ViewModelController

  //  m_component_provider->SetApplicationModel(nullptr);
  m_actions->OnImportFromFileRequest(file_name.toStdString());
  //  m_component_provider->SetApplicationModel(m_model.get());
  //  m_tree_view->expandAll();
}

}  // namespace sup::gui
