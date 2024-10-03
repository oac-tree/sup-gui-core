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

#include "waveform_editor_widget.h"

#include "waveform_table_widget.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>
#include <mvvm/views/chart_canvas.h>

#include <QSplitter>
#include <QVBoxLayout>

namespace sup::gui
{

WaveformEditorWidget::WaveformEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_splitter(new QSplitter)
    , m_chart_canvas(new mvvm::ChartCanvas)
    , m_table_widget(new WaveformTableWidget(nullptr))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_splitter->setOrientation(Qt::Vertical);
  m_splitter->addWidget(m_chart_canvas);
  m_splitter->addWidget(m_table_widget);

  layout->addWidget(m_splitter);
}

mvvm::LineSeriesItem *WaveformEditorWidget::GetLineSeriesItem()
{
  return m_table_widget->GetLineSeriesItem();
}

WaveformEditorWidget::~WaveformEditorWidget() = default;

void WaveformEditorWidget::SetLineSeriesItem(mvvm::LineSeriesItem *line_series_item)
{
  m_table_widget->SetLineSeriesItem(line_series_item);
}

void WaveformEditorWidget::SetViewportItem(mvvm::ChartViewportItem *viewport_item)
{
  m_chart_canvas->SetViewport(viewport_item);
}

mvvm::PointItem *WaveformEditorWidget::GetSelectedPoint()
{
  return m_table_widget->GetSelectedPoint();
}

void WaveformEditorWidget::ZoomIn()
{
  m_chart_canvas->ZoomIn();
}

void WaveformEditorWidget::ZoomOut()
{
  m_chart_canvas->ZoomOut();
}

void WaveformEditorWidget::SetViewportToContent()
{
  m_chart_canvas->SetViewportToContent();
}

void WaveformEditorWidget::SetSelectedPoint(const mvvm::PointItem *item)
{
  m_table_widget->SetSelectedPoint(item);
}

}  // namespace sup::gui
