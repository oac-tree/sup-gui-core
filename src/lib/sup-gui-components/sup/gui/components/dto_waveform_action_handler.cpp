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

#include "dto_waveform_action_handler.h"

#include <sup/gui/core/exceptions.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>

namespace sup::gui
{

DtoWaveformActionHandler::DtoWaveformActionHandler(DtoWaveformEditorContext context,
                                                   QObject *parent)
    : QObject(parent), m_context(std::move(context))
{
  if (!m_context.waveform_container)
  {
    throw RuntimeException("Callback to retrieve waveform container is not defined");
  }

  if (!m_context.data_container)
  {
    throw RuntimeException("Callback to retrieve waveform container is not defined");
  }

  if (!m_context.selected_waveform)
  {
    throw RuntimeException("Callback to retrieve selected waveform is not defined");
  }
}

bool DtoWaveformActionHandler::CanAddWaveform() const
{
  return GetWaveformContainer() != nullptr && GetDataContainer() != nullptr;
}

void DtoWaveformActionHandler::AddWaveform()
{
  if (!CanAddWaveform())
  {
    return;
  }

  auto waveform = InsertWaveform(std::make_unique<mvvm::LineSeriesItem>());
  InsertDataForWaveform(waveform);

  emit SelectWaveformRequest(waveform);
}

bool DtoWaveformActionHandler::CanRemoveWaveform() const
{
  return false;
}

void DtoWaveformActionHandler::RemoveWaveform()
{
  if (!CanRemoveWaveform())
  {
    return;
  }
}

mvvm::LineSeriesItem *DtoWaveformActionHandler::GetSelectedWaveform() const
{
  return m_context.selected_waveform();
}

mvvm::SessionItem *DtoWaveformActionHandler::GetWaveformContainer() const
{
  return m_context.waveform_container();
}

mvvm::SessionItem *DtoWaveformActionHandler::GetDataContainer() const
{
  return m_context.data_container();
}

mvvm::ISessionModel *DtoWaveformActionHandler::GetModel()
{
  // for the moment we assume that WaveformContainer and DataContainer are located in the same model
  return GetWaveformContainer()->GetModel();
}

mvvm::LineSeriesItem *DtoWaveformActionHandler::InsertWaveform(
    std::unique_ptr<mvvm::LineSeriesItem> waveform)
{
  auto result_ptr = waveform.get();

  // inserting right after current selection
  auto selected = GetSelectedWaveform();
  auto waveform_tag_index = selected ? selected->GetTagIndex().Next() : mvvm::TagIndex::Append();

  GetModel()->InsertItem(std::move(waveform), GetWaveformContainer(), waveform_tag_index);

  return result_ptr;
}

void DtoWaveformActionHandler::InsertDataForWaveform(mvvm::LineSeriesItem *waveform)
{
  auto waveform_tag_index = waveform->GetTagIndex();

  // data will be located in the data container nearby, the index of data items matches the index of
  // waveform items
  auto data_tag_index = mvvm::TagIndex::Default(waveform_tag_index.index);

  auto waveform_data =
      GetModel()->InsertItem<mvvm::LineSeriesDataItem>(GetDataContainer(), data_tag_index);

  waveform->SetDataItem(waveform_data);
}

}  // namespace sup::gui
