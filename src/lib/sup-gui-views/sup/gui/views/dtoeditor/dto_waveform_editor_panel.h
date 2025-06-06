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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_EDITOR_PANEL_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_EDITOR_PANEL_H_

#include <QWidget>

namespace mvvm
{
class LineSeriesItem;
class ChartViewportItem;
}  // namespace mvvm

namespace sup::gui
{

class WaveformEditorWidget;
class ItemStackWidget;
class WaveformModel;

/**
 * @brief The DtoWaveformEditorPanel class is a vertical panel with embedded single-waveform editor.
 *
 * Located on the center of DtoWaveformformView.
 */
class DtoWaveformEditorPanel : public QWidget
{
  Q_OBJECT
public:
  explicit DtoWaveformEditorPanel(QWidget* parent_widget = nullptr);
  ~DtoWaveformEditorPanel() override;

  /**
   * @brief Sets main viewport with waveforms.
   */
  void SetViewport(mvvm::ChartViewportItem* viewport);

  /**
   * @brief Set waveform to show in waveform editor.
   */
  void SetLineSeriesItem(mvvm::LineSeriesItem* line_series_item);

private:
  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
  WaveformEditorWidget* m_waveform_editor{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_EDITOR_PANEL_H_
