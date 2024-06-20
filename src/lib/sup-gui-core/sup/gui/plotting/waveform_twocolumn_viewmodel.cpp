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

#include "waveform_twocolumn_viewmodel.h"

#include "custom_children_waveform_strategies.h"
#include "custom_row_waveform_strategies.h"

#include <mvvm/viewmodel/viewmodel_controller.h>
#include <mvvm/viewmodel/viewmodel_controller_impl.h>

namespace
{
/**
 * @brief Creates implementation for ViewModelController with custom children and row strategies.
 */
std::unique_ptr<mvvm::IViewModelController> CreateImpl(mvvm::ViewModelBase *viewmodel)
{
  auto children_strategy = std::make_unique<sup::gui::WaveformChildrenStrategy>();
  auto row_strategy = std::make_unique<sup::gui::TwoColumRowStrategy>();

  auto result = std::make_unique<mvvm::ViewModelControllerImpl>(
      viewmodel, std::move(children_strategy), std::move(row_strategy));

  return result;
}

class WaveformTwoColumnViewModelController : public mvvm::ViewModelController
{
public:
  explicit WaveformTwoColumnViewModelController(mvvm::ViewModelBase *viewmodel)
      : mvvm::ViewModelController(CreateImpl(viewmodel))
  {
  }
};

}  // namespace

namespace sup::gui
{

WaveformTwoColumnViewModel::WaveformTwoColumnViewModel(mvvm::ISessionModel *model, QObject *parent)
    : ViewModel(parent)
{
  auto controller = std::make_unique<WaveformTwoColumnViewModelController>(this);
  controller->SetModel(model);
  SetController(std::move(controller));
}

int WaveformTwoColumnViewModel::columnCount(const QModelIndex &parent) const
{
  return 2;  // "x" and "y"
}

}  // namespace sup::gui
