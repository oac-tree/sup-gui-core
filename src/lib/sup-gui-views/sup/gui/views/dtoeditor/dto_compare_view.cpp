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

#include "configuration_splittable_widget.h"
#include "dto_compare_tools_panel.h"

#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/model/import_helper.h>
#include <sup/gui/widgets/message_helper.h>
#include <sup/gui/widgets/settings_splitter.h>

#include <mvvm/model/item_selection.h>

#include <QVBoxLayout>

namespace sup::gui
{

namespace
{

constexpr auto kSplitterSettingName("DtoCompareView/splitter");

}

DtoCompareView::DtoCompareView(ISettingsManager& settings_manager, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_splitter(new sup::gui::SettingsSplitter(kSplitterSettingName, settings_manager))
    , m_explorer_panel(new DtoCompareToolsPanel)
    , m_splittable_config_editor(new ConfigurationSplittableWidget(settings_manager))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);
  layout->setSpacing(0);
  layout->addWidget(m_splitter);

  m_splitter->addWidget(m_explorer_panel);
  m_splitter->addWidget(m_splittable_config_editor);

  m_splitter->RestoreState();

  SetupConnections();

  m_splittable_config_editor->CreatePanel();
  m_splittable_config_editor->SetPanelSyncAppearanceFlag(
      m_explorer_panel->GetPanelSyncAppearanceFlag());

  connect(m_explorer_panel, &DtoCompareToolsPanel::panelSyncAppearanceFlagChanged,
          m_splittable_config_editor, &ConfigurationSplittableWidget::SetPanelSyncAppearanceFlag);
}

DtoCompareView::~DtoCompareView() = default;

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

void DtoCompareView::SetupConnections()
{
  auto on_selection_changed = [this](const mvvm::ItemSelection& selection)
  {
    auto selected_config = selection.GetItem<ConfigurationItem>();
    m_splittable_config_editor->SetConfiguration(selected_config);
  };
  connect(m_explorer_panel, &DtoCompareToolsPanel::SelectionChanged, this, on_selection_changed);

  connect(m_splittable_config_editor, &ConfigurationSplittableWidget::focusPanelSelectionChanged,
          m_explorer_panel, &DtoCompareToolsPanel::SetSelection);
}

}  // namespace sup::gui
