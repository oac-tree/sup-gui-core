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

#ifndef SUP_GUI_PLOTTING_CUSTOM_ROW_WAVEFORM_STRATEGIES_H_
#define SUP_GUI_PLOTTING_CUSTOM_ROW_WAVEFORM_STRATEGIES_H_

#include <mvvm/viewmodel/abstract_row_strategy.h>

namespace sup::gui
{

/**
 * @brief The TwoColumRowStrategy class generates row of two x,y values representing time series
 * data.
 *
 * It's a part of WaveformTwoColumnViewModel.
 */
class TwoColumRowStrategy : public mvvm::AbstractRowStrategy
{
public:
  int GetSize() const override;

  QStringList GetHorizontalHeaderLabels() const override;

private:
  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRowImpl(mvvm::SessionItem* item) override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_PLOTTING_CUSTOM_ROW_WAVEFORM_STRATEGIES_H_
