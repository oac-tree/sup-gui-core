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

#include <QWidget>

class QToolBar;
class QSplitter;

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
class WaveformEditorActions;
class WaveformEditorActionHandler;
struct WaveformEditorContext;

/**
 * @brief The WaveformEditorWidget class is a waveform editor intended for editing of one waveform
 * at a time.
 *
 * It has a viewport at the top, and horizontal table with (x,y) points at the bottom.
 */
class WaveformEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WaveformEditorWidget(QWidget* parent = nullptr);
  ~WaveformEditorWidget() override;

  /**
   * @brief Returns current waveform being served by the table widget.
   */
  mvvm::LineSeriesItem* GetLineSeriesItem() const;

  /**
   * @brief Set waveform to show in table widget.
   *
   * For the moment table widget can show only one waveform.
   */
  void SetLineSeriesItem(mvvm::LineSeriesItem* line_series_item);

  /**
   * @brief Sets viewport with waveforms to show in a canvas.
   *
   * Viewport
   */
  void SetViewportItem(mvvm::ChartViewportItem* viewport_item);

  /**
   * @brief Returns AnyValueItem representing currently selected point.
   *
   * @details The point is a structure with x,y fields represented by a column in a table. Selection
   * of any of "x" or "y" cells, or both, will report point as selected.
   */
  mvvm::PointItem* GetSelectedPoint() const;

  void ZoomIn();

  void ZoomOut();

  void SetViewportToContent();

  /**
   * @brief Set point selected.
   */
  void SetSelectedPoint(const mvvm::PointItem* item);

  /**
   * @brief Returns context representing current widget state which is relevant for action handler.
   */
  WaveformEditorContext CreateActionContext() const;

private:
  void SetupConnections();

  /**
   * @brief Places table widget together with own toolbar.
   */
  std::unique_ptr<QWidget> CreateTableEnvelopWidget();

  WaveformEditorActionHandler* m_action_handler{nullptr};
  WaveformEditorActions* m_actions{nullptr};

  QSplitter* m_splitter{nullptr};
  mvvm::ChartCanvas* m_chart_canvas{nullptr};
  QToolBar* m_table_tool_bar{nullptr};
  WaveformTableWidget* m_table_widget{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_WIDGET_H_
