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

#include "waveform_editor_action_handler.h"

#include "waveform_editor_context.h"

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/point_item.h>

namespace sup::gui
{

WaveformEditorActionHandler::WaveformEditorActionHandler(WaveformEditorContext context,
                                                         QObject *parent)
    : QObject(parent), m_context(context)
{
}

WaveformEditorActionHandler::~WaveformEditorActionHandler() = default;

void WaveformEditorActionHandler::OnAddColumnBeforeRequest()
{
  if (auto parent = GetParent(); parent)
  {
    // auto point_item = GetSelectedPoint();
    // auto tag_index = point_item ? point_item->GetTagIndex() : mvvm::TagIndex::First();
    // auto new_point_item = CreatePointToPrepend(*GetLineSeries()->GetDataItem(), point_item);
    // auto item = GetModel()->InsertItem(std::move(new_point_item), parent, tag_index);
    // emit SelectItemRequest(item);
  }
}

void WaveformEditorActionHandler::OnAddColumnAfterRequest()
{
  if (auto parent = GetParent(); parent)
  {
    // auto point_item = GetSelectedPoint();
    // auto tag_index = point_item ? point_item->GetTagIndex().Next() : mvvm::TagIndex::Append();
    // auto new_point_item = CreatePointToAppend(*GetLineSeries()->GetDataItem(), point_item);
    // auto item = GetModel()->InsertItem(std::move(new_point_item), parent, tag_index);
    // emit SelectItemRequest(item);
  }
}

void WaveformEditorActionHandler::OnRemoveColumnRequest()
{
  if (auto point_item = GetSelectedPoint(); point_item)
  {
    auto item_to_select_next = mvvm::utils::FindNextItemToSelect(point_item);
    GetModel()->RemoveItem(point_item);
    emit SelectItemRequest(item_to_select_next);
  }
}

mvvm::ISessionModel *WaveformEditorActionHandler::GetModel()
{
  return GetLineSeries() ? GetLineSeries()->GetModel() : nullptr;
}

mvvm::LineSeriesItem *WaveformEditorActionHandler::GetLineSeries()
{
  return m_context.get_selected_waveform_callback();
}

mvvm::PointItem *WaveformEditorActionHandler::GetSelectedPoint()
{
  return m_context.get_selected_point_callback();
}

mvvm::SessionItem *WaveformEditorActionHandler::GetParent()
{
  return GetLineSeries() ? GetLineSeries()->GetDataItem() : nullptr;
}

}  // namespace sup::gui
