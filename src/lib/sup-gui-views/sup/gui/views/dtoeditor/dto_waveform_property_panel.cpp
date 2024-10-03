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

#include "dto_waveform_property_panel.h"

#include <sup/gui/widgets/item_stack_widget.h>

#include <mvvm/views/all_items_tree_view.h>

#include <QVBoxLayout>

namespace sup::gui
{

DtoWaveformPropertyPanel::DtoWaveformPropertyPanel(mvvm::ISessionModel* model, QWidget* parent)
    : QWidget(parent)
    , m_stack_widget(new sup::gui::ItemStackWidget)
    , m_tree_view(new mvvm::AllItemsTreeView)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_stack_widget->AddWidget(m_tree_view);

  layout->addWidget(m_stack_widget);
}

DtoWaveformPropertyPanel::~DtoWaveformPropertyPanel() = default;

}  // namespace sup::gui
