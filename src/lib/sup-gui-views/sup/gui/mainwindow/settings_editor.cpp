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

#include "settings_editor.h"

#include "common_settings_editor.h"
#include "settings_helper.h"
#include "stacked_widget_provider.h"

#include <sup/gui/model/persistent_settings_model.h>
#include <sup/gui/model/undo_settings_item.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/item_selection.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/viewmodel/top_items_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>
#include <mvvm/widgets/widget_utils.h>

#include <QListView>
#include <QSplitter>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace sup::gui
{

SettingsEditor::SettingsEditor(const mvvm::ISessionModel& model, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_splitter(new QSplitter)
    , m_list_view(new QListView)
    , m_stacked_widget(new QStackedWidget)
    , m_list_component_provider(mvvm::CreateProvider<mvvm::TopItemsViewModel>(m_list_view))
    , m_stacked_widget_provider(std::make_unique<StackedWidgetProvider>(m_stacked_widget))
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_splitter);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_splitter->addWidget(m_list_view);
  m_splitter->addWidget(m_stacked_widget);
  m_splitter->setSizes({200, 400});

  m_stacked_widget_provider->RegisterWidget<CommonSettingsEditor>(
      mvvm::GetTypeName<UndoSettingsItem>());
  m_stacked_widget_provider->RegisterDefaultWidget<CommonSettingsEditor>();

  connect(m_list_component_provider.get(), &mvvm::ItemViewComponentProvider::SelectionChanged, this,
          [this](const mvvm::ItemSelection& selection)
          { SetSettingsGroup(selection.GetItem()); });

  SetInitialValues(model);
}

SettingsEditor::~SettingsEditor() = default;

void SettingsEditor::SetSettingsGroup(mvvm::SessionItem* item)
{
  auto widget = m_stacked_widget_provider->SetCurrentWidget(item->GetType());
  if (auto item_widget = dynamic_cast<SessionItemWidget*>(widget); item_widget)
  {
    item_widget->SetItem(item);
  }
}

void SettingsEditor::PropagateSettingsToModel(mvvm::ISessionModel& model)
{
  model.ReplaceRootItem(mvvm::utils::CopyItem(*m_working_settings_copy->GetRootItem()));
}

void SettingsEditor::RegisterWidget(const std::string& key,
                                    const widget_factory_func_t& factory_func)
{
  m_stacked_widget_provider->RegisterWidget(key, factory_func);
}

void SettingsEditor::SetInitialValues(const mvvm::ISessionModel& model)
{
  m_working_settings_copy = model.Clone();

  m_list_component_provider->SetApplicationModel(m_working_settings_copy.get());
  auto top_item = mvvm::utils::GetTopItem(*m_working_settings_copy);
  m_list_component_provider->SetSelection(mvvm::ItemSelection(top_item));
}

}  // namespace sup::gui
