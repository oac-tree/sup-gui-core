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

#include "waveform_twocolumn_viewmodel.h"

#include "custom_children_waveform_strategies.h"
#include "custom_row_waveform_strategies.h"

#include <mvvm/viewmodel/viewmodel_controller_factory.h>

namespace sup::gui
{

WaveformTwoColumnViewModel::WaveformTwoColumnViewModel(mvvm::ISessionModel *model,
                                                       QObject *parent_object)
    : ViewModel(parent_object)
{
  auto controller = mvvm::factory::CreateController<sup::gui::WaveformChildrenStrategy,
                                                    sup::gui::TwoColumRowStrategy>(model, this);
  SetController(std::move(controller));
}

}  // namespace sup::gui
