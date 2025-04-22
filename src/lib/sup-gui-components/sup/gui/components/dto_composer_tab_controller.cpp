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

#include "dto_composer_tab_controller.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/signals/model_listener.h>

#include <QTabWidget>

namespace sup::gui
{

DtoComposerTabController::DtoComposerTabController(mvvm::ISessionModel *model,
                                                   create_widget_callback_t callback,
                                                   QTabWidget *tab_widget, QObject *parent_object)
    : QObject(parent_object)
    , m_model(model)
    , m_create_widget_callback(std::move(callback))
    , m_tab_widget(tab_widget)
{
  if (!tab_widget)
  {
    throw NullArgumentException("DtoComposerTabController: QTabWidget is not initialised");
  }

  m_listener = std::make_unique<mvvm::ModelListener>(model);
  m_listener->Connect<mvvm::ItemInsertedEvent>(this,
                                               &DtoComposerTabController::OnItemInsertedEvent);
  m_listener->Connect<mvvm::AboutToRemoveItemEvent>(
      this, &DtoComposerTabController::OnAboutToRemoveItemEvent);
  m_listener->Connect<mvvm::ModelResetEvent>(this, &DtoComposerTabController::OnModelResetEvent);

  InitTabs();
}

QWidget *DtoComposerTabController::GetWidgetForItem(const mvvm::SessionItem *container)
{
  auto iter = m_widget_map.find(container);
  return iter == m_widget_map.end() ? nullptr : iter->second;
}

void DtoComposerTabController::InitTabs()
{
  m_tab_widget->clear();
  for (auto child : m_model->GetRootItem()->GetAllItems())
  {
    InsertAnyValueItemContainerTab(child, child->GetTagIndex().GetIndex());
  }
}

DtoComposerTabController::~DtoComposerTabController() = default;

void DtoComposerTabController::OnItemInsertedEvent(const mvvm::ItemInsertedEvent &event)
{
  auto [parent, tag_index] = event;

  // we are interested only in root item with containers
  if (parent == m_model->GetRootItem())
  {
    auto container = parent->GetItem(tag_index);
    InsertAnyValueItemContainerTab(container, tag_index.GetIndex());
  }
}

void DtoComposerTabController::OnAboutToRemoveItemEvent(const mvvm::AboutToRemoveItemEvent &event)
{
  auto [parent, tag_index] = event;
  // we are interested only in root item with containers
  if (parent == m_model->GetRootItem())
  {
    auto container = parent->GetItem(tag_index);
    if (auto widget = GetWidgetForItem(container); widget)
    {
      m_tab_widget->removeTab(tag_index.GetIndex());
      delete widget;

      (void)m_widget_map.erase(container);
    }
    else
    {
      throw RuntimeException("DtoComposerTabController: can't find widget for container");
    }
  }
}

void DtoComposerTabController::InsertAnyValueItemContainerTab(mvvm::SessionItem *container,
                                                              int index)
{
  auto widget = m_create_widget_callback(container);

  (void)m_widget_map.insert({container, widget.get()});

  // ownership is taken by QTabWidget
  (void)m_tab_widget->insertTab(index, widget.release(), "AnyValue");
}

void DtoComposerTabController::OnModelResetEvent(const mvvm::ModelResetEvent &event)
{
  ClearWidgets();
  (void)event;
  // This is called when the project is reloaded from disk.
  // We do not call any tab initialization here, since other widgets might still receiving
  // ModelResetEvent event. SeeDtoEditorMainWindow::OnProjectLoad();
}

void DtoComposerTabController::ClearWidgets()
{
  m_tab_widget->clear();

  for (auto [item, widget] : m_widget_map)
  {
    delete widget;
  }
  m_widget_map.clear();
}

}  // namespace sup::gui
