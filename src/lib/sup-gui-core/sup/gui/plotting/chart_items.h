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

#ifndef SUP_GUI_PLOTTING_CHART_ITEMS_H_
#define SUP_GUI_PLOTTING_CHART_ITEMS_H_

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/compound_item.h>
#include <mvvm/standarditems/viewport_item.h>

namespace mvvm
{
class PenItem;
}

namespace sup::gui
{

/**
 * @brief The LineSeriesItem class represents 1D graph.
 *
 * It is a counterprart of QtChart::QLineSeries in mvvm world. It contains plot properties and a
 * link to AnyValueItem. LineSeriesItem is intended for plotting via ChartViewportItem.
 */
class LineSeriesItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "LineSeries";
  static inline const std::string kLink = "kLink";
  static inline const std::string kOffset = "kOffset";
  LineSeriesItem();

  void SetDataItem(AnyValueArrayItem* item);

  AnyValueArrayItem* GetDataItem();
  const AnyValueArrayItem* GetDataItem() const;

  /**
   * @brief Returns the value of x-axis offset.
   *
   * @details X-coordinates of all points during the plot will be shifted by this number. It doesn't
   * affect values itself.
   */
  double GetXOffset() const;

  /**
   * @brief Set the value of x-axis offset.
   *
   * @details X-coordinates of all points during the plot will be shifted by this number. It doesn't
   * affect values itself.
   */
  void SetXOffset(double value);

  /**
   * @brief Returns x-values.
   */
  std::vector<double> GetBinCenters() const;

  /**
   * @brief Returns y-values.
   */
  std::vector<double> GetValues() const;

  std::string GetNamedColor() const;

  void SetNamedColor(const std::string& named_color);

  mvvm::PenItem* GetPenItem() const;
};

/**
 * @brief The ChartViewportItem class is intended to show multiple LineSeriesItem.
 *
 * It is a counterpart of QtChart::QChart in mvvm world.
 */
class ChartViewportItem : public mvvm::ViewportItem
{
public:
  static inline const std::string Type = "ChartViewport";

  ChartViewportItem();

  std::vector<LineSeriesItem*> GetLineSeries() const;

protected:
  std::pair<double, double> GetDataXRange() const override;
  std::pair<double, double> GetDataYRange() const override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_PLOTTING_CHART_ITEMS_H_
