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

#include "dto_waveform_view.h"

#include "dto_waveform_editor_panel.h"
#include "dto_waveform_list_panel.h"
#include "dto_waveform_property_panel.h"

#include <sup/gui/model/waveform_model.h>

#include <mvvm/standarditems/line_series_item.h>

#include <QSplitter>
#include <QVBoxLayout>

namespace sup::gui
{

DtoWaveformView::DtoWaveformView(QWidget* parent)
    : QWidget(parent)
    , m_splitter(new QSplitter)
    , m_list_panel(new DtoWaveformListPanel)
    , m_editor_panel(new DtoWaveformEditorPanel)
    , m_property_panel(new DtoWaveformPropertyPanel)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_splitter->addWidget(m_list_panel);
  m_splitter->addWidget(m_editor_panel);
  m_splitter->addWidget(m_property_panel);
  m_splitter->setSizes({200, 300, 200});

  layout->addWidget(m_splitter);

  connect(m_list_panel, &DtoWaveformListPanel::WaveformSelected, this,
          &DtoWaveformView::SetLineSeriesItem);
}

DtoWaveformView::~DtoWaveformView() = default;

void DtoWaveformView::SetLineSeriesItem(mvvm::LineSeriesItem* line_series_item)
{
  m_editor_panel->SetLineSeriesItem(line_series_item);
  m_property_panel->SetLineSeriesItem(line_series_item);
}

void DtoWaveformView::SetWaveformModel(WaveformModel* model)
{
  m_list_panel->SetViewport(model->GetViewPort());
  m_editor_panel->SetViewport(model->GetViewPort());
}

}  // namespace sup::gui
