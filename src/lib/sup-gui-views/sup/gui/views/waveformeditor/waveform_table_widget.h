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

#ifndef SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_TABLE_WIDGET_H_
#define SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_TABLE_WIDGET_H_

#include <QWidget>
#include <memory>

class QTableView;

namespace mvvm
{
class ApplicationModel;
class SessionItem;
class LineSeriesItem;
class PointItem;
}  // namespace mvvm

namespace sup::gui
{

class WaveformTableComponentProvider;

/**
 * @brief The WaveformTableWidget class represents a widget with little table with line series
 * points.
 *
 * Located at the bottom of WaveformEditor, right under 1D plot.
 */

class WaveformTableWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WaveformTableWidget(mvvm::ApplicationModel* model, QWidget* parent_widget = nullptr);
  ~WaveformTableWidget() override;

  void SetModel(mvvm::ApplicationModel* model);

  /**
   * @brief Returns current waveform being served by the table widget.
   */
  mvvm::LineSeriesItem* GetLineSeriesItem();

  /**
   * @brief Set waveform to show in table widget.
   *
   * @details For the moment table widget can show only one waveform.
   */
  void SetLineSeriesItem(mvvm::LineSeriesItem* line_series_item);

  /**
   * @brief Returns AnyValueItem representing currently selected point.
   *
   * @details The point is a structure with x,y fields represented by a column in a table. Selection
   * of any of "x" or "y" cells, or both, will report point as selected.
   */
  mvvm::PointItem* GetSelectedPoint();

  /**
   * @brief Set point selected in table widget.
   *
   * @param item The AnyValueItem representing a point (struct with two fields).
   *
   * @details Will select two cells represening x,y values.
   */
  void SetSelectedPoint(const mvvm::PointItem* item);

  QSize sizeHint() const override;

private:
  QTableView* m_table_view{nullptr};

  mvvm::ApplicationModel* m_model{nullptr};
  std::unique_ptr<WaveformTableComponentProvider> m_component_provider;

  mvvm::LineSeriesItem* m_current_line_series{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_TABLE_WIDGET_H_
