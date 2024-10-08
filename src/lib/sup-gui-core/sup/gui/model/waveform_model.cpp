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

#include "waveform_model.h"

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/container_item.h>

namespace sup::gui
{

WaveformModel::WaveformModel(std::shared_ptr<mvvm::ItemPool> pool)
    : mvvm::ApplicationModel("WaveformModel", std::move(pool))

{
  PopulateModel();
}

void WaveformModel::Clear()
{
  mvvm::ApplicationModel::Clear();
  PopulateModel();
}

mvvm::ChartViewportItem *WaveformModel::GetViewPort() const
{
  return mvvm::utils::GetTopItem<mvvm::ChartViewportItem>(this);
}

mvvm::ContainerItem *WaveformModel::GetDataContainer() const
{
  return mvvm::utils::GetTopItem<mvvm::ContainerItem>(this);
}

void WaveformModel::PopulateModel()
{
  InsertItem<mvvm::ChartViewportItem>();
  InsertItem<mvvm::ContainerItem>();
}

}  // namespace sup::gui
