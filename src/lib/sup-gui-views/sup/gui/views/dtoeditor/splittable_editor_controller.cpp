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

#include "splittable_editor_controller.h"

#include "splittable_combo_panel.h"

#include <sup/gui/components/widget_focus_handler.h>
#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <QSplitter>

namespace sup::gui
{

SplittableEditorController::SplittableEditorController(QSplitter* splitter,
                                                       create_panel_func_t factory_func,
                                                       QObject* parent_object)
    : QObject(parent_object)
    , m_splitter(splitter)
    , m_factory_func(std::move(factory_func))
    , m_focus_handler(std::make_unique<WidgetFocusHandler<SplittableComboPanel>>())
{
  if (m_splitter == nullptr)
  {
    throw RuntimeException("Splitter is not initialized");
  }

  if (!m_factory_func)
  {
    throw RuntimeException("Factory function is not initialized");
  }
}

SplittableEditorController::~SplittableEditorController() = default;

SplittableComboPanel* SplittableEditorController::CreatePanel(SplittableComboPanel* after_widget)
{
  auto new_widget = CreateAndConnectSplittableComboPanel();
  auto new_widget_ptr = new_widget.get();

  if (after_widget == nullptr)
  {
    // append to the end
    m_splitter->addWidget(new_widget.release());
    m_focus_handler->AddWidget(new_widget_ptr);
  }
  else
  {
    // insert after the specified widget
    const std::int32_t index = m_splitter->indexOf(after_widget);
    m_splitter->insertWidget(index + 1, new_widget.release());
    m_focus_handler->AddWidget(new_widget_ptr, after_widget);
    new_widget_ptr->SetItem(after_widget->GetCurrentItem());
  }
  return new_widget_ptr;
}

void SplittableEditorController::ClosePanel(SplittableComboPanel* widget_to_close)
{
  if (widget_to_close == nullptr)
  {
    return;
  }
  m_focus_handler->RemoveWidget(widget_to_close);
  widget_to_close->hide();
  widget_to_close->deleteLater();
}

SplittableComboPanel* SplittableEditorController::GetFocusWidget()
{
  return m_focus_handler->GetFocusWidget();
}

void SplittableEditorController::SetFocusWidget(SplittableComboPanel* widget)
{
  m_focus_handler->SetFocusWidget(widget);
}

std::unique_ptr<SplittableComboPanel>
SplittableEditorController::CreateAndConnectSplittableComboPanel()
{
  auto result = CreateSplittableComboPanel();

  connect(result.get(), &SplittableComboPanel::splitPanelRequest, this,
          &SplittableEditorController::onSplitPanelRequest);
  connect(result.get(), &SplittableComboPanel::closePanelRequest, this,
          &SplittableEditorController::onClosePanelRequest);
  connect(result.get(), &SplittableComboPanel::focusPanelRequest, this,
          &SplittableEditorController::onFocusPanelRequest);
  connect(result.get(), &SplittableComboPanel::itemSelectionChanged, this,
          &SplittableEditorController::onItemSelectionChanged);

  return result;
}

std::unique_ptr<SplittableComboPanel> SplittableEditorController::CreateSplittableComboPanel()
{
  return m_factory_func();
}

void SplittableEditorController::onSplitPanelRequest()
{
  auto sending_panel = qobject_cast<SplittableComboPanel*>(sender());
  CreatePanel(/*add after*/ sending_panel);
}

void SplittableEditorController::onClosePanelRequest()
{
  auto sending_panel = qobject_cast<SplittableComboPanel*>(sender());
  ClosePanel(sending_panel);
}

void SplittableEditorController::onFocusPanelRequest()
{
  auto sending_panel = qobject_cast<SplittableComboPanel*>(sender());
  m_focus_handler->SetFocusWidget(sending_panel);
}

void SplittableEditorController::onItemSelectionChanged(mvvm::ItemSelection selection)
{
  auto sending_panel = qobject_cast<SplittableComboPanel*>(sender());
  if (sending_panel != GetFocusWidget())
  {
    return;
  }

  if (m_previous_selection_in_focus_widget != selection)
  {
    m_previous_selection_in_focus_widget = selection;
    emit focusPanelSelectionChanged(m_previous_selection_in_focus_widget);
  }
}

}  // namespace sup::gui
