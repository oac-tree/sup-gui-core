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

#ifndef SUP_GUI_PLOTTING_WAVEFORM_TWOCOLUMN_VIEWMODEL_H_
#define SUP_GUI_PLOTTING_WAVEFORM_TWOCOLUMN_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace sup::gui
{

/**
 * @brief The WaveformTwoColumnViewModel shows LineSeriesDataItem with waveform data on board as
 * two-columns table.
 *
 * @details The layout of LineSeriesDataItem:
 * @code
 * LineSeriesDataItem
 *  PointItem
 *    "x": 1.0
 *    "y": 10.0
 *  PointItem
 *    "x": 2.0
 *    "y": 20.0
 * @endcode
 *
 * Will become:
 * @code
 * 1.0 10.0
 * 2.0 20.0
 * @endcode
 */

class WaveformTwoColumnViewModel : public mvvm::ViewModel
{
  Q_OBJECT

public:
  explicit WaveformTwoColumnViewModel(mvvm::ISessionModel* model, QObject* parent = nullptr);

  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_PLOTTING_WAVEFORM_TWOCOLUMN_VIEWMODEL_H_
