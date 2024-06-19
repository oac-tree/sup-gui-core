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

#include "waveform_helper.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>

#include <sup/dto/anytype.h>

#include <stdexcept>

namespace
{

/**
 * @brief Create point slighly shifted to the right wrt the given point.
 */
std::unique_ptr<sup::gui::AnyValueItem> CreateShiftedPoint(const sup::gui::AnyValueItem* point,
                                                           double x_shift)
{
  if (point)
  {
    auto [x, y] = sup::gui::GetXY(*point);
    return sup::gui::CreatePoint(x + x_shift, y);
  }

  return sup::gui::CreatePoint(0.0, 0.0);
}

sup::gui::AnyValueItem* GetLastPoint(const sup::gui::AnyValueArrayItem& array_item)
{
  auto points = array_item.GetChildren();
  return points.empty() ? nullptr : points.back();
}

sup::gui::AnyValueItem* GetFirstPoint(const sup::gui::AnyValueArrayItem& array_item)
{
  auto points = array_item.GetChildren();
  return points.empty() ? nullptr : points.front();
}

/**
 * @brief Make pair of vectors instead of vector of pairs.
 */
std::pair<std::vector<double>, std::vector<double>> GetPairOfVectors(
    const std::vector<std::pair<double, double>>& points)
{
  std::vector<double> first;
  std::vector<double> second;

  auto on_element = [&first, &second](const auto& element)
  {
    first.push_back(element.first);
    second.push_back(element.second);
  };
  for_each(std::begin(points), std::end(points), on_element);

  return {first, second};
}

}  // namespace

namespace sup::gui
{

std::unique_ptr<AnyValueItem> CreatePoint(double x, double y)
{
  auto result = std::make_unique<AnyValueStructItem>();

  result->AddScalarField(kXFieldName, sup::dto::kFloat64TypeName, x);
  result->AddScalarField(kYFieldName, sup::dto::kFloat64TypeName, y);

  return result;
}

std::unique_ptr<AnyValueArrayItem> CreatePlotData(
    const std::vector<std::pair<double, double>>& data)
{
  auto result = std::make_unique<AnyValueArrayItem>();

  for (const auto& [x, y] : data)
  {
    // using utility function to provide notifications
    mvvm::utils::InsertItem(CreatePoint(x, y), result.get(), mvvm::TagIndex::Append());
  }

  return result;
}

std::pair<double, double> GetXY(const AnyValueItem& item)
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

std::unique_ptr<AnyValueItem> CreatePointToAppend(
    const AnyValueArrayItem& array_item, const AnyValueItem* selected_point)
{
  // creating point slighly shifted to the right
  auto base_point = selected_point ? selected_point : GetLastPoint(array_item);
  return CreateShiftedPoint(base_point, kDefaultDx);
}

std::unique_ptr<AnyValueItem> CreatePointToPrepend(
    const AnyValueArrayItem& array_item, const AnyValueItem* selected_point)
{
  // creating point slighly shifted to the left
  auto base_point = selected_point ? selected_point : GetFirstPoint(array_item);
  return CreateShiftedPoint(base_point, -kDefaultDx);
}

std::vector<std::pair<double, double>> GetPoints(const AnyValueArrayItem* array_item)
{
  if (!array_item)
  {
    return {};
  }
  std::vector<std::pair<double, double>> result;
  auto points = array_item->GetChildren();
  auto on_point = [](auto item) { return GetXY(*item); };
  std::transform(std::begin(points), std::end(points), std::back_inserter(result), on_point);
  return result;
}

std::vector<double> GetXValues(const sup::gui::AnyValueArrayItem *array_item)
{
  return GetPairOfVectors(GetPoints(array_item)).first;
}

std::vector<double> GetYValues(const sup::gui::AnyValueArrayItem* array_item)
{
  return GetPairOfVectors(GetPoints(array_item)).second;
}

}  // namespace pspsdemo
