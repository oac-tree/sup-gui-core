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

#include <sup/gui/model/settings_model.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/viewmodel/top_items_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>
#include <mvvm/views/property_flat_view.h>
#include <mvvm/widgets/widget_utils.h>

#include <QListView>
#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>

namespace sup::gui
{

SettingsEditor::SettingsEditor(const mvvm::ISessionModel&model, QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_splitter(new QSplitter)
    , m_list_view(new QListView)
    , m_settings_view(new mvvm::PropertyFlatView)
    , m_list_component_provider(mvvm::CreateProvider<mvvm::TopItemsViewModel>(m_list_view))
    , m_property_view_model(std::make_unique<mvvm::PropertyViewModel>(nullptr))
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_splitter);

  m_splitter->addWidget(m_list_view);
  m_splitter->addWidget(m_settings_view);
  m_splitter->setSizes({200, 400});

  connect(m_list_component_provider.get(), &mvvm::ItemViewComponentProvider::SelectedItemChanged,
          this, [this](auto item) { SetSettingsGroup(item); });

  m_settings_view->layout()->setContentsMargins(mvvm::utils::UnitSize(1), 0, 0, 0);

  SetInitialValues(model);
}

void SettingsEditor::SetInitialValues(const mvvm::ISessionModel &model)
{
  // in the absence of ISessionModel::Clone, we just clone root item
  m_settings_model = std::make_unique<SettingsModel>();
  m_settings_model->ReplaceRootItem(model.GetRootItem()->Clone());

  m_list_component_provider->SetApplicationModel(m_settings_model.get());
  m_settings_view->SetViewModel(m_property_view_model.get());
  m_list_component_provider->SetSelectedItem(m_settings_model->GetSettingsItems().at(0));
}

SettingsEditor::~SettingsEditor() = default;

void SettingsEditor::SetSettingsGroup(mvvm::SessionItem *item)
{
  m_property_view_model->SetRootSessionItem(item);
}

void SettingsEditor::WriteToPersistentStorage()
{
  WriteApplicationSettings(*m_settings_model);
}

}  // namespace sup::gui
