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

#include "pen_controller.h"

#include <mvvm/plotting/plot_helper.h>
#include <mvvm/standarditems/plottable_items.h>

#include <QLineSeries>
#include <stdexcept>

namespace sup::gui
{

PenController::PenController(QtCharts::QLineSeries *line_series) : m_qt_line_series(line_series)
{
  if (!line_series)
  {
    throw std::runtime_error("Error in PenController: uninitialized line series.");
  }
}

QtCharts::QLineSeries *PenController::GetQtLineSeries() const
{
  return m_qt_line_series;
}

void PenController::Subscribe()
{
  Connect<mvvm::PropertyChangedEvent>(this, &PenController::OnPropertyChanged);
  UpdateLineSeriesFromItem();
}

void PenController::OnPropertyChanged(const mvvm::PropertyChangedEvent &event)
{
  UpdateLineSeriesFromItem();
}

void PenController::UpdateLineSeriesFromItem()
{
  QColor color(QString::fromStdString(GetItem()->GetNamedColor()));

  QPen pen;
  pen.setColor(color);
  pen.setStyle(GetQtPenStyle(GetItem()));
  pen.setWidth(GetItem()->GetWidth());
  m_qt_line_series->setPen(pen);
}

}  // namespace pspsdemo
