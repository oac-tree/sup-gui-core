/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "settings_editor.h"

#include "settings_helper.h"
#include "common_settings_editor.h"

#include <sup/gui/model/settings_model.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/viewmodel/top_items_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>
#include <mvvm/widgets/widget_utils.h>

#include <QListView>
#include <QSplitter>
#include <QVBoxLayout>

namespace sup::gui
{

SettingsEditor::SettingsEditor(const mvvm::ISessionModel &model, QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_splitter(new QSplitter)
    , m_list_view(new QListView)
    , m_common_setting_editor(new CommonSettingsEditor)
    , m_list_component_provider(mvvm::CreateProvider<mvvm::TopItemsViewModel>(m_list_view))
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_splitter);

  m_splitter->addWidget(m_list_view);
  m_splitter->addWidget(m_common_setting_editor);
  m_splitter->setSizes({200, 400});

  connect(m_list_component_provider.get(), &mvvm::ItemViewComponentProvider::SelectedItemChanged,
          this, [this](auto item) { SetSettingsGroup(item); });

  SetInitialValues(model);
}

SettingsEditor::~SettingsEditor() = default;

void SettingsEditor::SetSettingsGroup(mvvm::SessionItem *item)
{
  m_common_setting_editor->SetItem(item);
}

void SettingsEditor::PropagateSettingsToModel(mvvm::ISessionModel &model)
{
  model.ReplaceRootItem(mvvm::utils::CopyItem(*m_working_settings_copy->GetRootItem()));
}

void SettingsEditor::SetInitialValues(const mvvm::ISessionModel &model)
{
  m_working_settings_copy = model.Clone();

  m_list_component_provider->SetApplicationModel(m_working_settings_copy.get());
  m_list_component_provider->SetSelectedItem(mvvm::utils::GetTopItem(*m_working_settings_copy));
}

}  // namespace sup::gui
