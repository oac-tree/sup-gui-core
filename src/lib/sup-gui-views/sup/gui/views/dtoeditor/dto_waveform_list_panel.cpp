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

#include "dto_waveform_list_panel.h"

#include <sup/gui/components/dto_waveform_action_handler.h>
#include <sup/gui/model/waveform_model.h>
#include <sup/gui/views/dtoeditor/dto_waveform_actions.h>
#include <sup/gui/widgets/item_stack_widget.h>

#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/viewmodel/top_items_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>

#include <QListView>
#include <QVBoxLayout>

namespace sup::gui
{

DtoWaveformListPanel::DtoWaveformListPanel(QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_action_handler(new DtoWaveformActionHandler(CreateContext(), this))
    , m_actions(new DtoWaveformActions(m_action_handler))
    , m_stack_widget(new sup::gui::ItemStackWidget)
    , m_list_view(new QListView)
    , m_component_provider(mvvm::CreateProvider<mvvm::TopItemsViewModel>(m_list_view))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(m_stack_widget);

  using ActionKey = DtoWaveformActions::ActionKey;
  m_list_view->setWindowTitle("Waveform list");
  m_stack_widget->AddWidget(
      m_list_view, m_actions->GetActions({ActionKey::kAddWaveform, ActionKey::kRemoveWaveform}));

  connect(m_component_provider.get(), &mvvm::ItemViewComponentProvider::SelectedItemChanged, this,
          [this](auto) { emit WaveformSelected(GetSelectedWaveform()); });

  connect(m_action_handler, &DtoWaveformActionHandler::SelectWaveformRequest, this,
          &DtoWaveformListPanel::SetSelectedWaveform);
}

void DtoWaveformListPanel::SetViewport(mvvm::ChartViewportItem *viewport)
{
  m_chart_viewport = viewport;
  m_component_provider->SetItem(viewport);
  if (viewport->GetLineSeriesCount() > 0)
  {
    SetSelectedWaveform(viewport->GetLineSeries().at(0));
  }
}

DtoWaveformListPanel::~DtoWaveformListPanel() = default;

mvvm::LineSeriesItem *DtoWaveformListPanel::GetSelectedWaveform()
{
  return m_component_provider->GetSelected<mvvm::LineSeriesItem>();
}

void DtoWaveformListPanel::SetSelectedWaveform(mvvm::LineSeriesItem *waveform)
{
  m_component_provider->SetSelectedItem(waveform);
}

DtoWaveformEditorContext DtoWaveformListPanel::CreateContext()
{
  DtoWaveformEditorContext result;

  result.waveform_container = [this]() { return GetWaveformModel()->GetViewPort(); };
  result.data_container = [this]() { return GetWaveformModel()->GetDataContainer(); };
  result.selected_waveform = [this]() { return GetSelectedWaveform(); };

  return result;
}

WaveformModel *DtoWaveformListPanel::GetWaveformModel()
{
  return dynamic_cast<WaveformModel *>(m_chart_viewport->GetModel());
}

}  // namespace sup::gui
