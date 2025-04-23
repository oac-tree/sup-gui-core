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

#ifndef SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_H_
#define SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_H_

#include <QWidget>
#include <memory>

class QToolBar;

namespace mvvm
{
class ApplicationModel;
class LineSeriesItem;
class LineSeriesDataItem;
class ChartViewportItem;
class QToolBar;
}  // namespace mvvm

namespace sup::gui
{

class WaveformEditorWidget;
class WaveformEditorActionHandler;
struct WaveformEditorContext;

/**
 * @brief The WaveformEditor class is a widget with a line series canvas, a two-column table, and a
 * toolbar for a single waveform editing.
 *
 * Internally the editor operates via mvvm::LineSeriesItem machinery. This editor has its model on
 * board and is intended to communicate with the outside world via GetWaveform, and SetWaveform
 * methods.
 */
class WaveformEditor : public QWidget
{
  Q_OBJECT

public:
  explicit WaveformEditor(QWidget* parent_widget = nullptr);
  ~WaveformEditor() override;

  void SetWaveform(const std::vector<std::pair<double, double>>& waveform,
                   const std::string& title);

  std::vector<std::pair<double, double>> GetWaveform() const;

private:
  std::unique_ptr<mvvm::ApplicationModel> m_model;

  QToolBar* m_tool_bar{nullptr};
  WaveformEditorWidget* m_editor_widget{nullptr};

  mvvm::LineSeriesItem* m_line_series_item{nullptr};
  mvvm::LineSeriesDataItem* m_line_series_data_item{nullptr};
  mvvm::ChartViewportItem* m_chart_viewport_item{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_WAVEFORMEDITOR_WAVEFORM_EDITOR_H_
