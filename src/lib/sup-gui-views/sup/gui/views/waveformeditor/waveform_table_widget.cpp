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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "waveform_table_widget.h"

#include <sup/gui/plotting/waveform_table_component_provider.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/point_item.h>
#include <mvvm/widgets/widget_utils.h>

#include <QHeaderView>
#include <QTableView>
#include <QVBoxLayout>

namespace sup::gui
{

WaveformTableWidget::WaveformTableWidget(mvvm::ApplicationModel *model, QWidget *parent)
    : QWidget(parent), m_table_view(new QTableView)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(m_table_view);

  SetModel(model);

  m_table_view->setAlternatingRowColors(true);
  m_table_view->horizontalHeader()->setFixedHeight(mvvm::utils::UnitSize(0.75));
  m_table_view->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

  // the only way to set background color for this widget, but not down to children
  setObjectName("WaveformTableWidget");
  setStyleSheet("QWidget#WaveformTableWidget{background-color:white;}");
  setAttribute(Qt::WA_StyledBackground, true);
}

WaveformTableWidget::~WaveformTableWidget() = default;

void WaveformTableWidget::SetModel(mvvm::ApplicationModel *model)
{
  m_model = model;
  m_component_provider = std::make_unique<WaveformTableComponentProvider>(model, m_table_view);
}

mvvm::LineSeriesItem *WaveformTableWidget::GetLineSeriesItem()
{
  return m_current_line_series;
}

void WaveformTableWidget::SetLineSeriesItem(mvvm::LineSeriesItem *line_series_item)
{
  m_current_line_series = line_series_item;
  m_component_provider->SetItem(line_series_item ? line_series_item->GetDataItem() : nullptr);
}

mvvm::PointItem *WaveformTableWidget::GetSelectedPoint()
{
  auto selected = m_component_provider->GetSelectedItem();
  auto parent_struct = selected ? selected->GetParent() : nullptr;
  return dynamic_cast<mvvm::PointItem *>(parent_struct);
}

void WaveformTableWidget::SetSelectedPoint(const mvvm::PointItem *item)
{
  // enough to select only x, will select the whole column
  auto points = item->GetAllItems();
  if (!points.empty())
  {
    auto x_cell = points.at(0);
    m_component_provider->SetSelectedItem(x_cell);

    // make sure cell is visible
    auto indexes = m_component_provider->GetViewIndexes(x_cell);
    if (!indexes.empty())
    {
      m_table_view->scrollTo(indexes.at(0));
    }
  }
}

QSize WaveformTableWidget::sizeHint() const
{
  // we want vertical size of the widget to be big enough to fit the table, but not bigger
  const auto approximate_line_thickness = mvvm::utils::UnitSize(0.05);
  const auto height = m_table_view->rowHeight(0) + m_table_view->rowHeight(1)
                      + m_table_view->horizontalHeader()->height() + approximate_line_thickness;
  return {800, height};
}

}  // namespace sup::gui
