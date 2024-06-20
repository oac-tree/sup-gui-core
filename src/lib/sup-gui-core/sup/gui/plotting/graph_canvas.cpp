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

#include "graph_canvas.h"

#include "chart_view.h"

#include <sup/gui/plotting/chart_items.h>
#include <sup/gui/plotting/chart_viewport_controller.h>

#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

namespace
{
const double kDefaultZoomInFactor = 1.5;
}

namespace sup::gui
{

GraphCanvas::GraphCanvas(QWidget *parent)
    : QWidget(parent), m_chart(new QtCharts::QChart), m_chart_view(new ChartView)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(m_chart_view);

  m_chart_view->setChart(m_chart);
  m_chart->setAnimationOptions(QtCharts::QChart::AllAnimations);

  connect(m_chart_view, &ChartView::OperationModeChanged, this, &GraphCanvas::OperationModeChanged);
}

GraphCanvas::~GraphCanvas() = default;

void GraphCanvas::SetViewport(ChartViewportItem *viewport)
{
  m_viewport = viewport;
  m_viewport_controller = std::make_unique<ChartViewportController>(m_chart);
  m_viewport_controller->SetItem(viewport);
  m_chart_view->setRenderHint(QPainter::Antialiasing);
}

void GraphCanvas::ZoomIn()
{
  m_chart->zoom(kDefaultZoomInFactor);
}

void GraphCanvas::ZoomOut()
{
  m_chart->zoom(1. / kDefaultZoomInFactor);
}

void GraphCanvas::SetViewportToContent()
{
  // add 10% empty gap on top, above all graphs
  m_viewport->SetViewportToContent(0.0, 0.1, 0.0, 0.0);
}

void GraphCanvas::SetOperationMode(int mode)
{
  m_chart_view->SetOperationMode(static_cast<ChartView::OperationMode>(mode));
}

}  // namespace sup::gui
