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

#include "dto_compare_view.h"

#include "dto_explorer_panel.h"
#include "splittable_editor_widget.h"

#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/model/import_helper.h>
#include <sup/gui/widgets/custom_splitter.h>
#include <sup/gui/widgets/message_helper.h>

#include <mvvm/model/item_selection.h>

#include <QVBoxLayout>

namespace sup::gui
{

namespace
{

constexpr auto kSplitterSettingName("DtoCompareView/splitter");

}

DtoCompareView::DtoCompareView(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_splitter(new sup::gui::CustomSplitter(kSplitterSettingName))
    , m_explorer_panel(new DtoExplorerPanel)
    , m_splittable_config_editor(new SplittableEditorWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);
  layout->setSpacing(0);
  layout->addWidget(m_splitter);

  m_splitter->addWidget(m_explorer_panel);
  m_splitter->addWidget(m_splittable_config_editor);

  ReadSettings();

  SetupConnections();

  m_splittable_config_editor->CreatePanel();
}

DtoCompareView::~DtoCompareView()
{
  WriteSettings();
}

void DtoCompareView::SetConfigurationModel(ConfigurationModel* model)
{
  m_configuration_model = model;

  if (m_configuration_model == nullptr)
  {
    return;
  }

  m_explorer_panel->SetConfigurationContainer(m_configuration_model->GetConfigurationContainer());
  m_splittable_config_editor->SetModel(m_configuration_model);
}

void DtoCompareView::OnImportFromFileRequest(const QString& file_name)
{
  if (m_configuration_model == nullptr)
  {
    return;
  }
  auto on_error = [](const MessageEvent& event) { SendWarningMessage(event); };

  AppendConfigurationFromFile(file_name.toStdString(), on_error,
                              *m_configuration_model->GetConfigurationContainer());
}

void DtoCompareView::ReadSettings()
{
  m_splitter->ReadSettings();
}

void DtoCompareView::WriteSettings()
{
  m_splitter->WriteSettings();
}

void DtoCompareView::SetupConnections()
{
  connect(m_explorer_panel, &DtoExplorerPanel::FileDoubleClicked, this,
          &DtoCompareView::OnImportFromFileRequest);

  auto on_selection_changed = [this](const mvvm::ItemSelection& selection)
  {
    auto selected_config = selection.GetSelected<ConfigurationItem>();
    m_splittable_config_editor->SetConfiguration(const_cast<ConfigurationItem*>(selected_config));
  };
  connect(m_explorer_panel, &DtoExplorerPanel::SelectionChanged, this, on_selection_changed);

  connect(m_splittable_config_editor, &SplittableEditorWidget::focusPanelSelectionChanged,
          m_explorer_panel, &DtoExplorerPanel::SetSelection);
}

}  // namespace sup::gui
