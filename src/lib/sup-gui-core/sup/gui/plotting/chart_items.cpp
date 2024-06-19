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

#include "chart_items.h"

#include "waveform_helper.h"

#include <mvvm/standarditems/linked_item.h>
#include <mvvm/standarditems/plottable_items.h>

namespace
{
const std::string kItems = "kItems";
const std::string kOffset = "kOffset";
const std::string kPen = "kPen";
const std::string kDisplayed = "kDisplayed";

const double kFallBackMin = 0.0;
const double kFallBackMax = 1.0;

//! Find min and max values along all data points in all graphs.
//! Function 'func' is used to run either through binCenters or binValues.

template <typename T>
auto GetMinMax(const std::vector<sup::gui::LineSeriesItem *> &graphs, T func)
{
  std::vector<double> values;
  for (auto graph : graphs)
  {
    const auto array = func(graph);
    std::copy(std::begin(array), std::end(array), std::back_inserter(values));
  }

  auto [xmin, xmax] = std::minmax_element(std::begin(values), std::end(values));
  return xmin != xmax ? std::make_pair(*xmin, *xmax) : std::make_pair(kFallBackMin, kFallBackMax);
}

}  // namespace

namespace sup::gui
{

// -------------------------------------------------------------------------------------------------
// LineSeriesItem
// -------------------------------------------------------------------------------------------------

LineSeriesItem::LineSeriesItem() : mvvm::CompoundItem(Type)
{
  AddProperty<mvvm::LinkedItem>(kLink).SetDisplayName("Link");
  AddProperty(kOffset, 0.0).SetDisplayName("Offset");

  auto &pen = AddProperty<mvvm::PenItem>(kPen);
  pen.SetDisplayName("Pen");
  pen.SetWidth(kDefaultLineWidth);
  pen.SetNamedColor(kDefaultLineColor);

  AddProperty(kDisplayed, true).SetDisplayName("Displayed");
}

void LineSeriesItem::SetDataItem(AnyValueArrayItem *item)
{
  GetItem<mvvm::LinkedItem>(kLink)->SetLink(item);
}

AnyValueArrayItem *LineSeriesItem::GetDataItem()
{
  return GetItem<mvvm::LinkedItem>(kLink)->Get<AnyValueArrayItem>();
}

const AnyValueArrayItem *LineSeriesItem::GetDataItem() const
{
  return GetItem<mvvm::LinkedItem>(kLink)->Get<AnyValueArrayItem>();
}

double LineSeriesItem::GetXOffset() const
{
  return Property<double>(kOffset);
}

void LineSeriesItem::SetXOffset(double value)
{
  SetProperty<double>(kOffset, value);
}

std::vector<double> LineSeriesItem::GetBinCenters() const
{
  auto result = GetXValues(GetDataItem());
  std::transform(result.begin(), result.end(), result.begin(),
                 [this](auto element) { return element += GetXOffset(); });
  return result;
}

std::vector<double> LineSeriesItem::GetValues() const
{
  return GetYValues(GetDataItem());
}

std::string LineSeriesItem::GetNamedColor() const
{
  return GetPenItem()->GetNamedColor();
}

void LineSeriesItem::SetNamedColor(const std::string &named_color)
{
  GetPenItem()->SetNamedColor(named_color);
}

mvvm::PenItem *LineSeriesItem::GetPenItem() const
{
  return GetItem<mvvm::PenItem>(kPen);
}

// -------------------------------------------------------------------------------------------------
// ChartViewportItem
// -------------------------------------------------------------------------------------------------

ChartViewportItem::ChartViewportItem() : mvvm::ViewportItem(Type)
{
  RegisterAxes();
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kItems, {LineSeriesItem::Type}),
              /*set_default*/ true);
}

std::vector<LineSeriesItem *> ChartViewportItem::GetLineSeries() const
{
  return GetItems<LineSeriesItem>(kItems);
}

std::pair<double, double> ChartViewportItem::GetDataXRange() const
{
  return GetMinMax(GetLineSeries(), [](auto graph) { return graph->GetBinCenters(); });
}

std::pair<double, double> ChartViewportItem::GetDataYRange() const
{
  return GetMinMax(GetLineSeries(), [](auto graph) { return graph->GetValues(); });
}

}  // namespace sup::gui
