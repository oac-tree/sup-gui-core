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

#ifndef SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_WIDGET_H_
#define SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_WIDGET_H_

class QSplitter;

#include <QWidget>

namespace mvvm
{
class SessionItem;
class LineSeriesItem;
class ChartViewportItem;
class ChartCanvas;
class PointItem;
}  // namespace mvvm

namespace sup::gui
{

class WaveformTableWidget;

class WaveformEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WaveformEditorWidget(QWidget* parent = nullptr);
  ~WaveformEditorWidget() override;

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
   * @brief Sets viewport with waveforms to show in a canvas.
   */
  void SetViewportItem(mvvm::ChartViewportItem* viewport_item);

  /**
   * @brief Returns AnyValueItem representing currently selected point.
   *
   * @details The point is a structure with x,y fields represented by a column in a table. Selection
   * of any of "x" or "y" cells, or both, will report point as selected.
   */
  mvvm::PointItem* GetSelectedPoint();

  void ZoomIn();

  void ZoomOut();

  void SetViewportToContent();

  /**
   * @brief Set point selected.
   */
  void SetSelectedPoint(const mvvm::PointItem* item);

private:
  QSplitter* m_splitter{nullptr};
  mvvm::ChartCanvas* m_chart_canvas{nullptr};
  WaveformTableWidget* m_table_widget{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_WIDGET_H_
