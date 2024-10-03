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

#include "dto_waveform_view.h"

#include "dto_waveform_editor_panel.h"
#include "dto_waveform_list_panel.h"
#include "dto_waveform_property_panel.h"

#include <QSplitter>
#include <QVBoxLayout>

namespace sup::gui
{

DtoWaveformView::DtoWaveformView(mvvm::ISessionModel* model, QWidget* parent)
    : QWidget(parent)
    , m_splitter(new QSplitter)
    , m_list_panel(new DtoWaveformListPanel(model))
    , m_editor_panel(new DtoWaveformEditorPanel(model))
    , m_property_panel(new DtoWaveformPropertyPanel(model))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_splitter->addWidget(m_list_panel);
  m_splitter->addWidget(m_editor_panel);
  m_splitter->addWidget(m_property_panel);
  m_splitter->setSizes({200, 300, 300});

  layout->addWidget(m_splitter);
}

DtoWaveformView::~DtoWaveformView() = default;

}  // namespace sup::gui
