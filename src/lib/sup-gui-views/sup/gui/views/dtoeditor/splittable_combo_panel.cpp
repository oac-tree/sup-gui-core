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

#include "splittable_combo_panel.h"

#include <sup/gui/components/item_display_name_combo_adapter.h>
#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/mainwindow/session_item_widget.h>
#include <sup/gui/widgets/combo_tool_bar.h>
#include <sup/gui/widgets/placeholder_widget.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/signals/model_listener.h>

#include <QApplication>
#include <QEvent>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace sup::gui
{

namespace
{
constexpr auto kPlaceholderText = "Select one of available configurations";
}  // namespace

SplittableComboPanel::SplittableComboPanel(std::unique_ptr<SessionItemWidget> item_editor,
                                           ItemListCallback item_list_callback,
                                           QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tool_bar(new ComboToolBar(CreateItemNamesCallback()))
    , m_stacked_widget(new QStackedWidget)
    , m_placeholder_widget(new PlaceholderWidget(kPlaceholderText))
    , m_item_editor_widget(item_editor.release())
    , m_item_list_callback(std::move(item_list_callback))
{
  if (!m_item_list_callback)
  {
    throw RuntimeException("Item list callback is not set");
  }

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_tool_bar);
  layout->addWidget(m_stacked_widget);

  m_stacked_widget->addWidget(m_placeholder_widget);
  m_stacked_widget->addWidget(m_item_editor_widget);

  SetPlaceholderMode(PlaceholderMode::kShowPlaceholder);
  SetupConnections();

  // to track mouse click activity in descendant widgets and request focus change
  qApp->installEventFilter(this);
}

SplittableComboPanel::~SplittableComboPanel() = default;

mvvm::ISessionModel* SplittableComboPanel::GetModel()
{
  return m_model;
}

void SplittableComboPanel::SetModel(mvvm::ISessionModel* model)
{
  m_model = model;
  if (m_model != nullptr)
  {
    SetupModelListener();
  }
  else
  {
    m_listener.reset();
  }

  m_tool_bar->SetCurrentIndex(0);
}

void SplittableComboPanel::SetItem(mvvm::SessionItem* item)
{
  ItemDisplayNameComboAdapter adapter(GetAvailableItems());
  m_tool_bar->SetCurrentIndex(adapter.GetComboIndexFromItem(item));

  // the combo index change will trigger the ShowConfigurationInEditor method, so no need to call it
  // here explicitly
}

mvvm::SessionItem* SplittableComboPanel::GetCurrentItem()
{
  return m_current_item;
}

std::vector<mvvm::SessionItem*> SplittableComboPanel::GetAvailableItems() const
{
  return m_item_list_callback();
}

void SplittableComboPanel::ShowAsActive(bool value)
{
  m_tool_bar->ShowAsActive(value);
}

void SplittableComboPanel::ShowAsLastEditor(bool value)
{
  m_tool_bar->ShowAsLastEditor(value);
}

bool SplittableComboPanel::eventFilter(QObject* watched, QEvent* event)
{
  // will invoke panelFocusRequest on any mouse click activity in descendant widgets, so the
  // controller will be able to set focus to this panel and remove focus from other panels.

  if (event->type() == QEvent::MouseButtonPress)
  {
    if (auto* widget = qobject_cast<QWidget*>(watched); (widget != nullptr) && isAncestorOf(widget))
    {
      emit panelFocusRequest();
    }
  }
  return QWidget::eventFilter(watched, event);
}

void SplittableComboPanel::SetPlaceholderMode(PlaceholderMode mode)
{
  m_stacked_widget->setCurrentIndex(static_cast<std::int32_t>(mode));
}

void SplittableComboPanel::SetupConnections()
{
  connect(m_tool_bar, &ComboToolBar::splitViewRequest, this,
          &SplittableComboPanel::splitViewRequest);
  connect(m_tool_bar, &ComboToolBar::closeViewRequest, this,
          &SplittableComboPanel::closeViewRequest);

  auto on_combo_index_changed = [this](auto index)
  {
    ItemDisplayNameComboAdapter adapter(GetAvailableItems());
    ShowItemInEditor(adapter.GetItemFromComboIndex(index));
  };
  connect(m_tool_bar, &ComboToolBar::selectItemRequest, this, on_combo_index_changed);
}

void SplittableComboPanel::SetupModelListener()
{
  m_listener = std::make_unique<mvvm::ModelListener>(m_model);

  auto on_data_changed = [this](const mvvm::DataChangedEvent& event)
  {
    if (event.item == m_current_item)
    {
      // assume that the display name might be changed and update the combo box text
      m_tool_bar->UpdateAppearance();
    }
  };
  m_listener->Connect<mvvm::DataChangedEvent>(on_data_changed);

  auto on_about_to_remove_item = [this](const mvvm::AboutToRemoveItemEvent& event)
  {
    auto item_to_remove = event.item->GetItem(event.tag_index);

    if ((item_to_remove == m_current_item)
        || mvvm::utils::IsItemAncestor(m_current_item, item_to_remove))
    {
      SetItem(nullptr);
    }
  };
  m_listener->Connect<mvvm::AboutToRemoveItemEvent>(on_about_to_remove_item);
}

void SplittableComboPanel::ShowItemInEditor(mvvm::SessionItem* item)
{
  if (m_current_item == item)
  {
    return;
  }

  m_current_item = item;

  SetPlaceholderMode(m_current_item == nullptr ? PlaceholderMode::kShowPlaceholder
                                               : PlaceholderMode::kShowEditor);
  m_item_editor_widget->SetItem(m_current_item);
  emit itemSelectionChanged(mvvm::ItemSelection(m_current_item));
}

std::function<QList<QString>()> SplittableComboPanel::CreateItemNamesCallback()
{
  return [this]()
  {
    const ItemDisplayNameComboAdapter adapter(GetAvailableItems());
    return adapter.GetAvailableItemNames();
  };
}

}  // namespace sup::gui
