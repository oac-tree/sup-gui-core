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

#include "anyvalue_editor_widget.h"

#include "anyvalue_editor_actions.h"
#include "anyvalue_editor_textpanel.h"
#include "anyvalue_editor_treepanel.h"

#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/components/anyvalue_editor_action_handler.h>
#include <sup/gui/components/tree_helper.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/style/style_helper.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_dialog.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_dialog_factory.h>
#include <sup/gui/widgets/item_stack_widget.h>
#include <sup/gui/widgets/message_helper.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/utils/file_utils.h>

#include <sup/dto/anyvalue.h>

#include <QAction>
#include <QClipboard>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMenu>
#include <QMimeData>
#include <QSettings>
#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{

/**
 * @brief Returns action keys intended for a toolbar.
 */
std::vector<sup::gui::AnyValueEditorActions::ActionKey> GetToolBarActionKeys()
{
  using ActionKey = sup::gui::AnyValueEditorActions::ActionKey;
  return {ActionKey::kInsertAfter, ActionKey::kInsertInto, ActionKey::kRemoveSelected,
          ActionKey::kMoveUp, ActionKey::kMoveDown};
}

const QString kGroupName("AnyValueEditor/");
const QString kCurrentWorkdirSettingName = kGroupName + "workdir";
const QString kSplitterSettingName = kGroupName + "splitter";
const QString kIsVisiblePanelSettingName = kGroupName + "json_panel";

}  // namespace

namespace sup::gui
{

AnyValueEditorWidget::AnyValueEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_action_handler(new AnyValueEditorActionHandler(CreateActionContext(), this))
    , m_actions(new AnyValueEditorActions(m_action_handler, this))
    , m_text_panel(new AnyValueEditorTextPanel)
    , m_tree_panel(new AnyValueEditorTreePanel)
    , m_left_panel(CreateLeftPanel())
    , m_right_panel(CreateRightPanel())
    , m_splitter(new QSplitter)
{
  auto layout = new QVBoxLayout(this);

  m_splitter->addWidget(m_left_panel);
  m_splitter->addWidget(m_right_panel);
  m_splitter->setCollapsible(0, false);

  layout->addWidget(m_splitter);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  SetupConnections();
  SetupWidgetActions();
  ReadSettings();

  m_actions->RegisterActionsForContext(AppRegisterWidgetUniqueId(this));
}

AnyValueEditorWidget::~AnyValueEditorWidget()
{
  WriteSettings();
  AppUnregisterWidgetUniqueId(this);
}

void AnyValueEditorWidget::SetAnyValueItemContainer(mvvm::SessionItem *container)
{
  m_action_handler->SetAnyValueItemContainer(container);
  m_tree_panel->SetAnyValueItemContainer(container);
  m_text_panel->SetAnyValueItemContainer(container);
}

void AnyValueEditorWidget::OnImportFromFileRequest()
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

void AnyValueEditorWidget::OnExportToFileRequest()
{
  auto file_name = QFileDialog::getSaveFileName(
      this, "Save File", m_current_workdir + "/untitled.json", tr("Images (*.json *.JSON)"));

  if (!file_name.isEmpty())
  {
    m_action_handler->OnExportToFileRequest(file_name.toStdString());
    UpdateCurrentWorkdir(file_name);
  }
}

void AnyValueEditorWidget::OnImportWaveformRequest()
{
  auto dialog = CreateWaveformEditorDialog(GetTopItem(), this);
  if (dialog->exec() == QDialog::Accepted)
  {
    SetInitialValue(*dialog->GetResult());
  }
}

sup::gui::AnyValueItem *AnyValueEditorWidget::GetSelectedItem() const
{
  return m_tree_panel->GetSelectedItem();
}

void AnyValueEditorWidget::SetInitialValue(const AnyValueItem &item)
{
  m_action_handler->SetInitialValue(item);
}

AnyValueItem *AnyValueEditorWidget::GetTopItem()
{
  return m_action_handler->GetTopItem();
}

std::unique_ptr<AnyValueItem> AnyValueEditorWidget::GetResult()
{
  if (auto top_item = GetTopItem(); top_item)
  {
    return mvvm::utils::CloneItem(*top_item);
  }

  return {};
}

void AnyValueEditorWidget::ReadSettings()
{
  const QSettings settings;
  m_current_workdir = settings.value(kCurrentWorkdirSettingName, QDir::homePath()).toString();

  if (settings.contains(kSplitterSettingName))
  {
    m_splitter->restoreState(settings.value(kSplitterSettingName).toByteArray());
  }

  m_text_panel_is_visible = settings.value(kIsVisiblePanelSettingName, true).toBool();
  m_right_panel->setVisible(m_text_panel_is_visible);
}

void AnyValueEditorWidget::WriteSettings()
{
  QSettings settings;
  settings.setValue(kCurrentWorkdirSettingName, m_current_workdir);
  settings.setValue(kSplitterSettingName, m_splitter->saveState());
  settings.setValue(kIsVisiblePanelSettingName, m_text_panel_is_visible);
}

//! Set up all connections.

void AnyValueEditorWidget::SetupConnections()
{
  // selection request from action handler
  connect(m_action_handler, &AnyValueEditorActionHandler::SelectItemRequest, m_tree_panel,
          &AnyValueEditorTreePanel::SetSelected);

  // main editing request from AnyValueEditorActions
  connect(m_actions, &AnyValueEditorActions::ImportFromFileRequest, this,
          &AnyValueEditorWidget::OnImportFromFileRequest);

  // export request from text panel
  connect(m_text_panel, &AnyValueEditorTextPanel::ExportToFileRequest, this,
          &AnyValueEditorWidget::OnExportToFileRequest);

  connect(m_tree_panel->GetTreeView(), &QTreeView::customContextMenuRequested, this,
          &AnyValueEditorWidget::OnContextMenuRequest);

  // selection change from tree panel
  connect(m_tree_panel, &AnyValueEditorTreePanel::SelectedItemChanged, this,
          [this](auto) { m_actions->UpdateEnabledStatus(); });
}

void AnyValueEditorWidget::SetupWidgetActions()
{
  m_show_right_sidebar = new QAction("Show/hide Right Sidebar", this);
  m_show_right_sidebar->setShortcut(QKeySequence(QString("Ctrl+Shift+0")));
  m_show_right_sidebar->setStatusTip("Show/hide Right Sidebar");
  m_show_right_sidebar->setIcon(utils::GetIcon("dock-right"));
  connect(m_show_right_sidebar, &QAction::triggered, this,
          [this](auto)
          {
            m_text_panel_is_visible = !m_text_panel_is_visible;
            m_right_panel->setVisible(m_text_panel_is_visible);
          });

  sup::gui::AppRegisterAction(sup::gui::constants::kViewMenu, m_show_right_sidebar);
}

AnyValueEditorContext AnyValueEditorWidget::CreateActionContext() const
{
  AnyValueEditorContext result;

  result.get_selected_callback = [this]() { return GetSelectedItem(); };
  result.send_message_callback = [](const auto &event) { sup::gui::SendWarningMessage(event); };
  result.get_mime_data = []() { return QGuiApplication::clipboard()->mimeData(); };
  result.set_mime_data = [](std::unique_ptr<QMimeData> data)
  { QGuiApplication::clipboard()->setMimeData(data.release()); };

  return result;
}

//! Updates cached value for last working directory for later saving in widget's persistent settings
void AnyValueEditorWidget::UpdateCurrentWorkdir(const QString &file_name)
{
  auto parent_path = mvvm::utils::GetParentPath(file_name.toStdString());
  m_current_workdir = QString::fromStdString(parent_path);
}

void AnyValueEditorWidget::OnContextMenuRequest(const QPoint &point)
{
  QMenu menu;
  menu.setToolTipsVisible(true);

  m_actions->SetupMenu(menu);

  auto collapse_menu = menu.addMenu("Tree settings");
  sup::gui::SetupCollapseExpandMenu(point, *collapse_menu, *m_tree_panel->GetTreeView());
  menu.exec(m_tree_panel->GetTreeView()->mapToGlobal(point));
}

QWidget *AnyValueEditorWidget::CreateLeftPanel()
{
  auto result = new ItemStackWidget;
  result->AddWidget(m_tree_panel, m_actions->GetActions(GetToolBarActionKeys()));
  return result;
}

QWidget *AnyValueEditorWidget::CreateRightPanel()
{
  auto result = new ItemStackWidget;
  result->AddWidget(m_text_panel, m_text_panel->actions());
  return result;
}

void AnyValueEditorWidget::ImportAnyValueFromFile(const QString &file_name)
{
  m_action_handler->OnImportFromFileRequest(file_name.toStdString());
  m_tree_panel->GetTreeView()->expandAll();
}

}  // namespace sup::gui
