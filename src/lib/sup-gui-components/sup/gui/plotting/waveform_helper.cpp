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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "waveform_helper.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/point_item.h>
#include <mvvm/standarditems/waveform_helper.h>
#include <mvvm/widgets/widget_utils.h>

#include <sup/dto/anytype.h>

#include <stdexcept>

namespace
{

/**
 * @brief Create point slightly shifted to the right wrt the given point.
 */
std::unique_ptr<mvvm::PointItem> CreateShiftedPoint(const mvvm::PointItem* point, double x_shift)
{
  auto result = std::make_unique<mvvm::PointItem>();

  if (point)
  {
    result->SetX(point->GetX() + x_shift);
    result->SetY(point->GetY());
  }

  return result;
}

mvvm::PointItem* GetLastPoint(const mvvm::LineSeriesDataItem& data_item)
{
  return data_item.GetPointCount() == 0 ? nullptr
                                        : data_item.GetPoint(data_item.GetPointCount() - 1);
}

mvvm::PointItem* GetFirstPoint(const mvvm::LineSeriesDataItem& data_item)
{
  return data_item.GetPointCount() == 0 ? nullptr : data_item.GetPoint(0);
}

/**
 * @brief Return named color corresponding to a given index.
 *
 * First 6 items will get always the same color, then we will use random color.
 */
std::string GetNextNamedColor(std::size_t index)
{
  // Colors from https://www.w3.org/TR/css-color-3/#svg-color
  static std::vector<std::string> colors = {"dodgerblue", "green",   "darkorange",
                                            "crimson",    "fuchsia", "teal"};

  return index < colors.size() ? colors[index] : mvvm::utils::RandomNamedColor();
}

}  // namespace

namespace sup::gui
{

std::unique_ptr<AnyValueItem> CreateFromPoint(double x, double y)
{
  auto result = std::make_unique<AnyValueStructItem>();

  result->AddScalarField(kXFieldName, sup::dto::kFloat64TypeName, x);
  result->AddScalarField(kYFieldName, sup::dto::kFloat64TypeName, y);

  return result;
}

std::pair<double, double> GetPoint(const AnyValueItem& item)
{
  auto children = item.GetChildren();
  if (!item.IsStruct() || children.size() != 2)
  {
    throw std::runtime_error("Error in GetXY: this item can't represent a point");
  }

  auto x = children.at(0)->Data();
  auto y = children.at(1)->Data();

  if (std::holds_alternative<double>(x) && std::holds_alternative<double>(y))
  {
    return {std::get<double>(x), std::get<double>(y)};
  }

  if (std::holds_alternative<float>(x) && std::holds_alternative<float>(y))
  {
    return {std::get<float>(x), std::get<float>(y)};
  }

  throw std::runtime_error("Error in GetXY: can't get float or double values from point");
}

std::unique_ptr<AnyValueArrayItem> CreateFromWaveform(
    const std::vector<std::pair<double, double>>& data)
{
  auto result = std::make_unique<AnyValueArrayItem>();

  for (const auto& [x, y] : data)
  {
    // using utility function to provide notifications
    mvvm::utils::InsertItem(CreateFromPoint(x, y), result.get(), mvvm::TagIndex::Append());
  }

  return result;
}

std::vector<std::pair<double, double>> GetWaveform(const AnyValueArrayItem* array_item)
{
  if (!array_item)
  {
    return {};
  }
  std::vector<std::pair<double, double>> result;
  auto points = array_item->GetChildren();
  auto on_point = [](auto item) { return GetPoint(*item); };
  std::transform(std::begin(points), std::end(points), std::back_inserter(result), on_point);
  return result;
}

std::unique_ptr<mvvm::PointItem> CreatePointToAppend(const mvvm::LineSeriesDataItem& data_item,
                                                     const mvvm::PointItem* selected_point)
{
  // creating point slighly shifted to the right
  auto base_point = selected_point ? selected_point : GetLastPoint(data_item);
  return CreateShiftedPoint(base_point, kDefaultDx);
}

std::unique_ptr<mvvm::PointItem> CreatePointToPrepend(const mvvm::LineSeriesDataItem& data_item,
                                                      const mvvm::PointItem* selected_point)
{
  // creating point slighly shifted to the left
  auto base_point = selected_point ? selected_point : GetFirstPoint(data_item);
  return CreateShiftedPoint(base_point, -kDefaultDx);
}

void SetupNewWaveform(mvvm::LineSeriesItem& item, int total_waveform_count)
{
  item.SetDisplayName("waveform" + std::to_string(total_waveform_count));
  item.SetFlag(mvvm::Appearance::kEditableDisplayName, true);
  item.SetNamedColor(GetNextNamedColor(total_waveform_count));
}

}  // namespace sup::gui
