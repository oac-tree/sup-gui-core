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

#include "dto_waveform_list_panel.h"

#include <sup/gui/components/dto_waveform_action_handler.h>
#include <sup/gui/model/waveform_model.h>
#include <sup/gui/views/dtoeditor/dto_waveform_actions.h>
#include <sup/gui/widgets/item_stack_widget.h>

#include <mvvm/providers/item_view_component_provider.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/viewmodel/top_items_viewmodel.h>

#include <QListView>
#include <QVBoxLayout>

namespace sup::gui
{

DtoWaveformListPanel::DtoWaveformListPanel(WaveformModel *model, QWidget *parent)
    : QWidget(parent)
    , m_model(model)
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
  m_stack_widget->AddWidget(
      m_list_view, m_actions->GetActions({ActionKey::kAddWaveform, ActionKey::kRemoveWaveform}));

  connect(m_component_provider.get(), &mvvm::ItemViewComponentProvider::SelectedItemChanged, this,
          [this](auto) { emit WaveformSelected(GetSelectedWaveform()); });

  connect(m_action_handler, &DtoWaveformActionHandler::SelectWaveformRequest, this,
          &DtoWaveformListPanel::SetSelectedWaveform);

  m_component_provider->SetItem(model->GetViewPort());
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

  result.waveform_container = [this]() { return m_model->GetViewPort(); };
  result.data_container = [this]() { return m_model->GetDataContainer(); };
  result.selected_waveform = [this]() { return GetSelectedWaveform(); };

  return result;
}

}  // namespace sup::gui
