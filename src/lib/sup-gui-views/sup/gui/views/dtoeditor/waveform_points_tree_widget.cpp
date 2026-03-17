/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include <mvvm/model/item_selection.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/point_item.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>

#include <QTreeView>
#include <QVBoxLayout>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(const mvvm::PointItem*)
#endif

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

  auto on_selection_changed = [this](const mvvm::ItemSelection& selection)
  {
    auto point = const_cast<mvvm::PointItem*>(selection.GetSelected<mvvm::PointItem>());
    emit pointSelected(point);
  };
  connect(m_component_provider.get(), &::mvvm::ItemViewComponentProvider::SelectionChanged, this,
          on_selection_changed);
}

WaveformPointsTreeWidget::~WaveformPointsTreeWidget() = default;

void WaveformPointsTreeWidget::SetLineSeriesItem(mvvm::LineSeriesItem* line_series_item)
{
  m_component_provider->SetItem((line_series_item == nullptr) ? nullptr
                                                              : line_series_item->GetDataItem());
}

void WaveformPointsTreeWidget::SetPointSelected(mvvm::PointItem* item)
{
  m_component_provider->SetSelection(mvvm::ItemSelection(item));
}

}  // namespace sup::gui
