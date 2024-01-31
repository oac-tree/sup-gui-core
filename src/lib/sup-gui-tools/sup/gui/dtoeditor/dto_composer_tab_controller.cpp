/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "dto_composer_tab_controller.h"

#include <sup/gui/anyvalueeditor/anyvalue_editor_widget.h>
#include <sup/gui/core/exceptions.h>

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/signals/model_listener.h>

#include <QTabWidget>

namespace sup::gui
{

DtoComposerTabController::DtoComposerTabController(mvvm::SessionModelInterface *model,
                                                   QTabWidget *tab_widget, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_tab_widget(tab_widget)
    , m_listener(std::make_unique<mvvm::ModelListener<mvvm::SessionModelInterface>>(model))
{
  if (!tab_widget)
  {
    throw NullException("DtoComposerTabController: QTabWidget is not initialised");
  }

  m_listener->Connect<mvvm::ItemInsertedEvent>(this,
                                               &DtoComposerTabController::OnItemInsertedEvent);
  m_listener->Connect<mvvm::AboutToRemoveItemEvent>(
      this, &DtoComposerTabController::OnAboutToRemoveItemEvent);

  InitTabs();
}

AnyValueEditorWidget *DtoComposerTabController::GetWidgetForItem(const mvvm::SessionItem *container)
{
  auto iter = m_widget_map.find(container);
  return iter == m_widget_map.end() ? nullptr : iter->second;
}

void DtoComposerTabController::InitTabs()
{
  for (auto child : m_model->GetRootItem()->GetAllItems())
  {
    InsertAnyValueItemContainerTab(child, child->GetTagIndex().index);
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
    InsertAnyValueItemContainerTab(container, tag_index.index);
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
      m_tab_widget->removeTab(tag_index.index);
      delete widget;

      m_widget_map.erase(container);
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
  auto widget =
      std::make_unique<AnyValueEditorWidget>(dynamic_cast<mvvm::ApplicationModel *>(m_model));

  widget->SetAnyValueItemContainer(container);
  m_widget_map.insert({container, widget.get()});

  // ownership is taken by QTabWidget
  m_tab_widget->insertTab(index, widget.release(), "AnyValue");
}

}  // namespace sup::gui
