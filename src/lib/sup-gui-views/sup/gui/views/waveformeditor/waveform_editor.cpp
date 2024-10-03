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

#include "waveform_editor_widget.h"

#include <sup/gui/widgets/style_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/point_item.h>

#include <QSplitter>
#include <QToolBar>
#include <QVBoxLayout>

namespace sup::gui
{

WaveformEditor::WaveformEditor(QWidget *parent)
    : QWidget(parent)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_tool_bar(new QToolBar)
    , m_editor_widget(new WaveformEditorWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_tool_bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_tool_bar->setIconSize(sup::gui::utils::ToolBarIconSize());
  m_tool_bar->addActions(m_editor_widget->actions());

  layout->addWidget(m_tool_bar);
  layout->addWidget(m_editor_widget);

  // In current implementation we create a single viewport, single LineSeriesItem and data for it.
  // Later we will manipulate data points via WaveformEditor::SetWaveform method.
  m_chart_viewport_item = m_model->InsertItem<mvvm::ChartViewportItem>();
  m_line_series_data_item = m_model->InsertItem<mvvm::LineSeriesDataItem>();

  m_line_series_item = m_model->InsertItem<mvvm::LineSeriesItem>(m_chart_viewport_item);
  m_line_series_item->SetDataItem(m_line_series_data_item);

  m_editor_widget->SetViewportItem(m_chart_viewport_item);
  m_editor_widget->SetLineSeriesItem(m_line_series_item);
}

WaveformEditor::~WaveformEditor() = default;

void WaveformEditor::SetWaveform(const std::vector<std::pair<double, double> > &waveform,
                                 const std::string &title)
{
  m_line_series_data_item->SetWaveform(waveform);
  m_line_series_item->SetDisplayName(title);
  m_editor_widget->SetViewportToContent();
}

std::vector<std::pair<double, double> > WaveformEditor::GetWaveform() const
{
  return m_line_series_data_item->GetWaveform();
}

}  // namespace sup::gui
