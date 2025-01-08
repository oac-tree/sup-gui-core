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

#include "dto_waveform_property_panel.h"

#include <sup/gui/widgets/item_stack_widget.h>

#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/views/all_items_tree_view.h>
#include <mvvm/views/property_tree_view.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace sup::gui
{

DtoWaveformPropertyPanel::DtoWaveformPropertyPanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_stack_widget(new sup::gui::ItemStackWidget)
    , m_plot_property_tree(new mvvm::AllItemsTreeView)
    , m_waveform_tree(new mvvm::AllItemsTreeView)
    , m_viewport_property_tree(new mvvm::PropertyTreeView)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_plot_property_tree->setWindowTitle("Waveform plot properties");
  m_waveform_tree->setWindowTitle("Waveform points");
  m_viewport_property_tree->setWindowTitle("Viewport properties");

  m_stack_widget->AddWidget(m_plot_property_tree);
  m_stack_widget->AddWidget(m_waveform_tree);
  m_stack_widget->AddWidget(m_viewport_property_tree);

  layout->addWidget(m_stack_widget);
}

DtoWaveformPropertyPanel::~DtoWaveformPropertyPanel() = default;

void DtoWaveformPropertyPanel::SetLineSeriesItem(mvvm::LineSeriesItem* line_series_item)
{
  m_plot_property_tree->SetItem(line_series_item);
  m_waveform_tree->SetItem(line_series_item ? line_series_item->GetDataItem() : nullptr);
  m_viewport_property_tree->SetItem(line_series_item ? line_series_item->GetParent() : nullptr);
  m_viewport_property_tree->GetTreeView()->setRootIsDecorated(true);
}

}  // namespace sup::gui
