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

#ifndef SUP_GUI_PLOTTING_CUSTOM_CHILDREN_STRATEGIES_H_
#define SUP_GUI_PLOTTING_CUSTOM_CHILDREN_STRATEGIES_H_

#include <mvvm/viewmodel/i_children_strategy.h>

namespace sup::gui
{

/**
 * @brief The WaveformChildrenStrategy class reports children of LineSeriesDataItem representing
 * time series.
 *
 * It's a part of WaveformTwoColumnViewModel. The strategy allows to iterate through points,
 * without going down to points's properties (i.e. "x" and "y").
 */
class WaveformChildrenStrategy : public mvvm::IChildrenStrategy
{
public:
  std::vector<mvvm::SessionItem *> GetChildren(const mvvm::SessionItem *item) const override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_PLOTTING_CUSTOM_CHILDREN_STRATEGIES_H_
