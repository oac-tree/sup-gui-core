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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "waveform_points_tree_widget.h"

#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace sup::gui
{

WaveformPointsTreeWidget::WaveformPointsTreeWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tree_view(new QTreeView)
    , m_component_provider(mvvm::CreateProvider<mvvm::AllItemsViewModel>(m_tree_view))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_tree_view);
}

void WaveformPointsTreeWidget::SetLineSeriesItem(mvvm::LineSeriesItem* line_series_item)
{
  m_component_provider->SetItem((line_series_item == nullptr) ? nullptr
                                                              : line_series_item->GetDataItem());
}

}  // namespace sup::gui
