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

#include "waveform_table_component_provider.h"

#include <sup/gui/plotting/waveform_twocolumn_viewmodel.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>

#include <QTableView>
#include <QTransposeProxyModel>

namespace sup::gui
{

WaveformTableComponentProvider::WaveformTableComponentProvider(mvvm::ApplicationModel *model,
                                                               QTableView *view)
    : mvvm::ItemViewComponentProvider(std::make_unique<WaveformTwoColumnViewModel>(model), view)
{
  AddProxyModel(std::make_unique<QTransposeProxyModel>());
  SetSelectionFlags(QItemSelectionModel::SelectCurrent | QItemSelectionModel::Columns);
}

int WaveformTableComponentProvider::GetSelectedPointIndex() const
{
  auto item = GetSelectedItem();
  // returning parent's index
  return item ? item->GetParent()->GetTagIndex().index : -1;
}
}  // namespace sup::gui
