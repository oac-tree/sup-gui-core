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

#include "waveform_editor_view.h"

#include "waveform_table_widget.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/plotting/chart_items.h>
#include <sup/gui/plotting/graph_canvas.h>

#include <mvvm/model/application_model.h>

#include <QSplitter>
#include <QVBoxLayout>

namespace sup::gui
{

WaveformEditorView::WaveformEditorView(QWidget *parent)
    : QWidget(parent)
    , m_splitter(new QSplitter)
    , m_graph_canvas(new GraphCanvas)
    , m_table_widget(new WaveformTableWidget(nullptr))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_splitter->setOrientation(Qt::Vertical);
  m_splitter->addWidget(m_graph_canvas);
  m_splitter->addWidget(m_table_widget);

  layout->addWidget(m_splitter);
}

LineSeriesItem *WaveformEditorView::GetLineSeriesItem()
{
  return m_table_widget->GetLineSeriesItem();
}

WaveformEditorView::~WaveformEditorView() = default;

void WaveformEditorView::SetLineSeriesItem(LineSeriesItem *line_series_item)
{
  m_table_widget->SetLineSeriesItem(line_series_item);
}

void WaveformEditorView::SetViewportItem(ChartViewportItem *viewport_item)
{
  m_graph_canvas->SetViewport(viewport_item);
}

AnyValueItem *WaveformEditorView::GetSelectedPoint()
{
  return m_table_widget->GetSelectedPoint();
}

void WaveformEditorView::ZoomIn()
{
  m_graph_canvas->ZoomIn();
}

void WaveformEditorView::ZoomOut()
{
  m_graph_canvas->ZoomOut();
}

void WaveformEditorView::SetViewportToContent()
{
  m_graph_canvas->SetViewportToContent();
}

void WaveformEditorView::SetSelectedPoint(const AnyValueItem *item)
{
  m_table_widget->SetSelectedPoint(item);
}

}  // namespace sup::gui
