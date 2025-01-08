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

#include "dto_waveform_editor_panel.h"

#include <sup/gui/model/waveform_model.h>
#include <sup/gui/views/waveformeditor/waveform_editor_widget.h>
#include <sup/gui/widgets/item_stack_widget.h>

#include <QVBoxLayout>

namespace sup::gui
{

DtoWaveformEditorPanel::DtoWaveformEditorPanel(QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_stack_widget(new sup::gui::ItemStackWidget)
    , m_waveform_editor(new WaveformEditorWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_waveform_editor->setWindowTitle("Waveform editor");
  m_stack_widget->AddWidget(m_waveform_editor, m_waveform_editor->actions());

  layout->addWidget(m_stack_widget);
}

void DtoWaveformEditorPanel::SetViewport(mvvm::ChartViewportItem *viewport)
{
  m_waveform_editor->SetViewportItem(viewport);
}

DtoWaveformEditorPanel::~DtoWaveformEditorPanel() = default;

void DtoWaveformEditorPanel::SetLineSeriesItem(mvvm::LineSeriesItem *line_series_item)
{
  m_waveform_editor->SetLineSeriesItem(line_series_item);
}

}  // namespace sup::gui
