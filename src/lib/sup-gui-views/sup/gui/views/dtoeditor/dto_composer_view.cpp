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

#include "dto_composer_view.h"

#include "dto_composer_tools_panel.h"

#include <sup/gui/app/app_view_context.h>
#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_widget.h>
#include <sup/gui/widgets/settings_splitter.h>

#include <mvvm/model/item_selection.h>

#include <QVBoxLayout>

namespace sup::gui
{

DtoComposerView::DtoComposerView(const AppViewContext& context, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_command_service(context.command_service)
    , m_splitter(new SettingsSplitter("DtoComposerView/splitter", context.settings_manager))
    , m_tools_panel(new DtoComposerToolsPanel)
    , m_anyvalue_editor(new AnyValueEditorWidget(context))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_splitter->addWidget(m_tools_panel);
  m_splitter->addWidget(m_anyvalue_editor);

  layout->addWidget(m_splitter);

  auto on_selection_changed = [this](const mvvm::ItemSelection& selection)
  {
    auto selected_config = selection.GetItem<ConfigurationItem>();
    m_anyvalue_editor->SetAnyValueItemContainer(selected_config);
  };
  connect(m_tools_panel, &DtoComposerToolsPanel::SelectionChanged, this, on_selection_changed);

  m_splitter->RestoreState();
}

DtoComposerView::~DtoComposerView() = default;

void DtoComposerView::SetConfigurationModel(ConfigurationModel* model)
{
  m_configuration_model = model;

  if (m_configuration_model == nullptr)
  {
    return;
  }

  m_tools_panel->SetConfigurationContainer(m_configuration_model->GetConfigurationContainer());
}

}  // namespace sup::gui
