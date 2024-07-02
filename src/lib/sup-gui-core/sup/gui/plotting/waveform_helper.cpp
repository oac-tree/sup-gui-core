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
#include <mvvm/standarditems/waveform_helper.h>

#include <sup/dto/anytype.h>

#include <stdexcept>

namespace
{

/**
 * @brief Create point slightly shifted to the right wrt the given point.
 */
std::unique_ptr<sup::gui::AnyValueItem> CreateShiftedPoint(const sup::gui::AnyValueItem* point,
                                                           double x_shift)
{
  if (point)
  {
    auto [x, y] = sup::gui::GetPoint(*point);
    return sup::gui::CreateFromPoint(x + x_shift, y);
  }

  return sup::gui::CreateFromPoint(0.0, 0.0);
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

std::unique_ptr<AnyValueItem> CreatePointToAppend(const AnyValueArrayItem& array_item,
                                                  const AnyValueItem* selected_point)
{
  // creating point slighly shifted to the right
  auto base_point = selected_point ? selected_point : GetLastPoint(array_item);
  return CreateShiftedPoint(base_point, kDefaultDx);
}

std::unique_ptr<AnyValueItem> CreatePointToPrepend(const AnyValueArrayItem& array_item,
                                                   const AnyValueItem* selected_point)
{
  // creating point slighly shifted to the left
  auto base_point = selected_point ? selected_point : GetFirstPoint(array_item);
  return CreateShiftedPoint(base_point, -kDefaultDx);
}

}  // namespace sup::gui
