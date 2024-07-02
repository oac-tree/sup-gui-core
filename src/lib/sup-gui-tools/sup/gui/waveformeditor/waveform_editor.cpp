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

#include "waveform_editor.h"

#include "waveform_editor_toolbar.h"
#include "waveform_editor_view.h"

#include <sup/gui/plotting/waveform_editor_action_handler.h>
#include <sup/gui/plotting/waveform_editor_context.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/point_item.h>

#include <QSplitter>
#include <QVBoxLayout>

namespace sup::gui
{

WaveformEditor::WaveformEditor(QWidget *parent)
    : QWidget(parent)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_action_handler(std::make_unique<WaveformEditorActionHandler>(CreateActionContext()))
    , m_editor_view(new WaveformEditorView)
    , m_tool_bar(new WaveformEditorToolBar)
{
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_editor_view);
  layout->addWidget(m_tool_bar);

  SetupConnections();

  m_chart_viewport_item = m_model->InsertItem<mvvm::ChartViewportItem>();
  m_line_series_data_item = m_model->InsertItem<mvvm::LineSeriesDataItem>();

  m_line_series_item = m_model->InsertItem<mvvm::LineSeriesItem>(m_chart_viewport_item);
  m_line_series_item->SetDataItem(m_line_series_data_item);

  m_editor_view->SetViewportItem(m_chart_viewport_item);
}

void WaveformEditor::SetWaveform(const std::vector<std::pair<double, double> > &waveform, const std::string &title)
{
  m_line_series_data_item->SetWaveform(waveform);
  m_line_series_item->SetDisplayName(title);
}

std::vector<std::pair<double, double> > WaveformEditor::GetWaveform() const
{
  return m_line_series_data_item->GetWaveform();
}

WaveformEditor::~WaveformEditor() = default;

void WaveformEditor::SetupConnections()
{
  connect(m_tool_bar, &WaveformEditorToolBar::ZoomInRequest, m_editor_view,
          &WaveformEditorView::ZoomIn);
  connect(m_tool_bar, &WaveformEditorToolBar::ZoomOutRequest, m_editor_view,
          &WaveformEditorView::ZoomOut);
  connect(m_tool_bar, &WaveformEditorToolBar::SetViewportToContentRequest, m_editor_view,
          &WaveformEditorView::SetViewportToContent);

  connect(m_tool_bar, &WaveformEditorToolBar::AddColumnBeforeRequest, m_action_handler.get(),
          &WaveformEditorActionHandler::OnAddColumnBeforeRequest);
  connect(m_tool_bar, &WaveformEditorToolBar::AddColumnAfterRequest, m_action_handler.get(),
          &WaveformEditorActionHandler::OnAddColumnAfterRequest);
  connect(m_tool_bar, &WaveformEditorToolBar::RemoveColumnRequest, m_action_handler.get(),
          &WaveformEditorActionHandler::OnRemoveColumnRequest);

  connect(m_action_handler.get(), &WaveformEditorActionHandler::SelectItemRequest, this,
          [this](auto item)
          { m_editor_view->SetSelectedPoint(dynamic_cast<const mvvm::PointItem *>(item)); });
}

WaveformEditorContext WaveformEditor::CreateActionContext() const
{
  auto get_current_line_series = [this]() { return m_editor_view->GetLineSeriesItem(); };

  auto get_selected_point_callback = [this]() { return m_editor_view->GetSelectedPoint(); };
  return {get_current_line_series, get_selected_point_callback};
}

}  // namespace sup::gui
