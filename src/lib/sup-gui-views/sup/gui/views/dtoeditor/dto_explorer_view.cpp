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

#include "dto_explorer_view.h"

#include "dto_explorer_panel.h"

#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/model/import_helper.h>
#include <sup/gui/views/codeeditor/code_view.h>
#include <sup/gui/widgets/item_stack_widget.h>
#include <sup/gui/widgets/message_helper.h>
#include <sup/gui/widgets/settings_splitter.h>

#include <mvvm/utils/bin_utils.h>

#include <QVBoxLayout>

namespace sup::gui
{

namespace
{
constexpr auto kSplitterSettingName = "DtoExplorerView/splitter";
}

DtoExplorerView::DtoExplorerView(ISettingsManager& settings_manager, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_explorer_panel(new DtoExplorerPanel(settings_manager))
    , m_json_view(new sup::gui::CodeView(CodeView::kJSON, settings_manager))
    , m_right_panel(new sup::gui::ItemStackWidget)
    , m_splitter(new sup::gui::SettingsSplitter(kSplitterSettingName, settings_manager))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);

  m_splitter->addWidget(m_explorer_panel);

  m_right_panel->AddWidget(m_json_view);
  m_splitter->addWidget(m_right_panel);

  layout->addWidget(m_splitter);
  m_splitter->RestoreState();

  connect(m_explorer_panel, &DtoExplorerPanel::FileClicked, this,
          &DtoExplorerView::ShowFileContent);
  connect(m_explorer_panel, &DtoExplorerPanel::FileDoubleClicked, this,
          &DtoExplorerView::OnImportFromFileRequest);
}

DtoExplorerView::~DtoExplorerView() = default;

void DtoExplorerView::SetConfigurationModel(ConfigurationModel* model)
{
  m_configuration_model = model;

  if (m_configuration_model == nullptr)
  {
    return;
  }

  m_explorer_panel->SetConfigurationContainer(m_configuration_model->GetConfigurationContainer());
}

void DtoExplorerView::ShowFileContent(const QString& file_name)
{
  if (mvvm::utils::IsBinaryFile(file_name.toStdString()))
  {
    m_json_view->ClearText();
  }
  else
  {
    m_json_view->SetFile(file_name);
  }
}

void DtoExplorerView::OnImportFromFileRequest(const QString& file_name)
{
  if (m_configuration_model == nullptr)
  {
    return;
  }
  auto on_error = [](const MessageEvent& event) { SendWarningMessage(event); };

  AppendConfigurationFromFile(file_name.toStdString(), on_error,
                              *m_configuration_model->GetConfigurationContainer());
}

}  // namespace sup::gui
