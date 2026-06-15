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

#include "splittable_editor_widget.h"

#include "config_tab_widget.h"
#include "splittable_combo_panel.h"
#include "splittable_editor_controller.h"

#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>

#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>

#include <QSplitter>
#include <QVBoxLayout>

namespace sup::gui
{

SplittableEditorWidget::SplittableEditorWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_splitter(new QSplitter)
    , m_splitter_controller(CreateSplitterController())
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_splitter);

  connect(m_splitter_controller.get(), &SplittableEditorController::focusPanelSelectionChanged,
          this, &SplittableEditorWidget::focusPanelSelectionChanged);
}

SplittableEditorWidget::~SplittableEditorWidget() = default;

void SplittableEditorWidget::SetModel(ConfigurationModel* model)
{
  m_model = model;
  m_splitter_controller->SetModel(model);
}

void SplittableEditorWidget::SetConfiguration(ConfigurationItem* config)
{
  if (auto focus_widget = m_splitter_controller->GetFocusWidget(); focus_widget)
  {
    focus_widget->SetItem(config);
  }
}

void SplittableEditorWidget::CreatePanel()
{
  m_splitter_controller->CreatePanel();
}

SplittableEditorController* SplittableEditorWidget::GetController() const
{
  return m_splitter_controller.get();
}

std::unique_ptr<SplittableEditorController> SplittableEditorWidget::CreateSplitterController() const
{
  auto factory_func = [this]() -> std::unique_ptr<SplittableComboPanel>
  {
    auto main_editor = std::make_unique<ConfigTabWidget>();

    auto item_list_callback = [this]() -> std::vector<mvvm::SessionItem*>
    {
      return m_model == nullptr ? std::vector<mvvm::SessionItem*>()
                                : mvvm::utils::CastItems<mvvm::SessionItem>(
                                      m_model->GetConfigurationContainer()->GetConfigurations());
    };

    auto result =
        std::make_unique<SplittableComboPanel>(std::move(main_editor), item_list_callback);
    result->SetModel(m_model);
    return result;
  };
  return std::make_unique<SplittableEditorController>(m_splitter, factory_func);
}

}  // namespace sup::gui
