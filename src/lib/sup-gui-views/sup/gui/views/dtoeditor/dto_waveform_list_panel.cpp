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

#include "dto_waveform_list_panel.h"

#include <sup/gui/widgets/item_stack_widget.h>

#include <QListView>
#include <QVBoxLayout>

namespace sup::gui
{

DtoWaveformListPanel::DtoWaveformListPanel(mvvm::ISessionModel *model, QWidget *parent)
    : QWidget(parent), m_stack_widget(new sup::gui::ItemStackWidget), m_list_view(new QListView)

{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(m_stack_widget);

  m_stack_widget->AddWidget(m_list_view);
}

DtoWaveformListPanel::~DtoWaveformListPanel() = default;

}  // namespace sup::gui
