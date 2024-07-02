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

#ifndef SUP_GUI_PLOTTING_WAVEFORM_HELPER_H_
#define SUP_GUI_PLOTTING_WAVEFORM_HELPER_H_

//! @file
//! Collection of helper functions to construct waveforms.

#include <memory>
#include <string>
#include <vector>

namespace mvvm
{
class PointItem;
class LineSeriesDataItem;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueItem;
class AnyValueArrayItem;

const std::string kXFieldName = "x";
const std::string kYFieldName = "y";

const int kDefaultLineWidth = 2;
const std::string kDefaultLineColor("#209fdf");  // between royal blue and steel blue

//! Constant to create points slighly shifted wrt currently selected.
const double kDefaultDx = 0.1;

/**
 * @brief Create AnyValueItem representing a point.
 *
 * @param x The value of x-axis.
 * @param y The value of y-axis.
 *
 * @return AnyValueItem struct.
 */
std::unique_ptr<AnyValueItem> CreateFromPoint(double x, double y);

/**
 * @brief Returns (x,y) from AnyValueItem structure representing a point.
 */
std::pair<double, double> GetPoint(const AnyValueItem& item);

/**
 * @brief Returns AnyValueArrayItem representing waveform.
 *
 * It will be an array of struct, where each struct has two float64 fields for "x" and "y" values.
 *
 * @param data Vector of (x,y) values.
 * @return AnyValueItem array.
 */
std::unique_ptr<AnyValueArrayItem> CreateFromWaveform(
    const std::vector<std::pair<double, double>>& data);

/**
 * @brief Return vector of (x,y) points from given AnyValueItemArray.
 */
std::vector<std::pair<double, double>> GetWaveform(const AnyValueArrayItem* array_item);

/**
 * @brief Create a point which can be added after the given point.
 *
 * Will create a point with reasonable default values of (x,y) coordinates. See code for more
 * details.
 *
 * @param data_item A LineSeriesDataItem that should get a new point.
 * @param selected_point Currenlty selected point.
 *
 * @return New point to add into array.
 * */
std::unique_ptr<mvvm::PointItem> CreatePointToAppend(const mvvm::LineSeriesDataItem& data_item,
                                                     const mvvm::PointItem* selected_point);

/**
 * @brief Create a point which can be added before the given point.
 *
 * Will create a point with reasonable default values of (x,y) coordinates. See code for more
 * details.
 *
 * @param array_item An array that should get a new point.
 * @param selected_point Currenlty selected point.
 *
 * @return New point to add into array.
 */
std::unique_ptr<mvvm::PointItem> CreatePointToPrepend(const mvvm::LineSeriesDataItem& data_item,
                                                      const mvvm::PointItem* selected_point);

}  // namespace sup::gui

#endif  // SUP_GUI_PLOTTING_WAVEFORM_HELPER_H_
