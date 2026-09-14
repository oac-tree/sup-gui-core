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

#include "configuration_splittable_widget.h"

#include "configuration_panel_mirror_controller.h"
#include "configuration_tab_widget.h"
#include "splittable_combo_panel.h"
#include "splittable_editor_controller.h"

#include <sup/gui/components/component_types.h>
#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>

#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>

#include <QSplitter>
#include <QVBoxLayout>

namespace sup::gui
{

ConfigurationSplittableWidget::ConfigurationSplittableWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_splitter(new QSplitter)
    , m_splitter_controller(CreateSplitterController())
    , m_panel_sync_controller(std::make_unique<ConfigurationPanelMirrorController>())
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_splitter);

  connect(m_splitter_controller.get(), &SplittableEditorController::focusPanelSelectionChanged,
          this, &ConfigurationSplittableWidget::focusPanelSelectionChanged);

  connect(m_splitter_controller.get(), &SplittableEditorController::newPanelAdded,
          m_panel_sync_controller.get(), &ConfigurationPanelMirrorController::ProcessPanelAdd);

  connect(m_splitter_controller.get(), &SplittableEditorController::aboutToClosePanel,
          m_panel_sync_controller.get(), &ConfigurationPanelMirrorController::ProcessPanelRemove);

  connect(m_splitter_controller.get(), &SplittableEditorController::referencePanelChanged,
          m_panel_sync_controller.get(), &ConfigurationPanelMirrorController::SetReferencePanel);
}

ConfigurationSplittableWidget::~ConfigurationSplittableWidget() = default;

void ConfigurationSplittableWidget::SetModel(ConfigurationModel* model)
{
  m_model = model;
  m_splitter_controller->SetModel(model);
}

void ConfigurationSplittableWidget::SetConfiguration(ConfigurationItem* config)
{
  if (auto focus_widget = m_splitter_controller->GetFocusWidget(); focus_widget)
  {
    focus_widget->SetItem(config);
  }
}

void ConfigurationSplittableWidget::SetPanelSyncAppearanceFlag(bool value)
{
  m_panel_sync_controller->SetActive(value);
}

void ConfigurationSplittableWidget::CreatePanel()
{
  m_splitter_controller->CreatePanel();
}

SplittableEditorController* ConfigurationSplittableWidget::GetController() const
{
  return m_splitter_controller.get();
}

ConfigurationPanelMirrorController* ConfigurationSplittableWidget::GetPanelMirrorController() const
{
  return m_panel_sync_controller.get();
}

std::unique_ptr<SplittableEditorController>
ConfigurationSplittableWidget::CreateSplitterController() const
{
  auto factory_func = [this]() -> std::unique_ptr<SplittableComboPanel>
  {
    auto main_editor = std::make_unique<ConfigurationTabWidget>();

    auto item_list_callback = [this]() -> std::vector<mvvm::SessionItem*>
    {
      return m_model == nullptr ? std::vector<mvvm::SessionItem*>()
                                : mvvm::utils::ConvertItems<mvvm::SessionItem>(
                                      m_model->GetConfigurationContainer()->GetConfigurations());
    };

    auto result = std::make_unique<SplittableComboPanel>(
        ComparePanelType::kReference, std::move(main_editor), item_list_callback);
    result->SetModel(m_model);
    return result;
  };
  return std::make_unique<SplittableEditorController>(m_splitter, factory_func);
}

}  // namespace sup::gui
