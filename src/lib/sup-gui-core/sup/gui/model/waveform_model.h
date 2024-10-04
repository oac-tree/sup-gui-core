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

#ifndef SUP_GUI_MODEL_WAVEFORM_MODEL_H_
#define SUP_GUI_MODEL_WAVEFORM_MODEL_H_

#include <mvvm/model/application_model.h>

namespace mvvm
{
class ContainerItem;
class ChartViewportItem;
}  // namespace mvvm

namespace sup::gui
{

/**
 * @brief The WaveformModel class is the main model for editing multiple waveforms in the context of
 * DtoWaveformView.
 *
 * For the moment the model by default contain an empty viewport, and empty data container.
 *
 *    ChartViewportItem        <-- viewport
 *      LineSeriesItem0        <-- waveform0
 *      LineSeriesItem1        <-- waveform1
 *    ContainerItem            <-- container for data
 *      LineSeriesDataItem0    <-- data for waveform0
 *      LineSeriesDataItem1    <-- data for waveform1
 */
class WaveformModel : public mvvm::ApplicationModel
{
public:
  explicit WaveformModel(std::shared_ptr<mvvm::ItemPool> pool = {});

  void Clear() override;

  mvvm::ChartViewportItem* GetViewPort() const;

  mvvm::ContainerItem* GetDataContainer() const;

private:
  /**
   * @brief Populate the model with the initial content.
   */
  void PopulateModel();
};

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_WAVEFORM_MODEL_H_
