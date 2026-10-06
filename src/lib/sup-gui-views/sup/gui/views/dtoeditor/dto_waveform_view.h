/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_VIEW_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_VIEW_H_

#include <QWidget>

namespace mvvm
{
class LineSeriesItem;
}

namespace sup::gui
{

class DtoWaveformListPanel;
class DtoWaveformEditorPanel;
class DtoWaveformPropertyPanel;
class WaveformModel;
class AppCommandContext;
class IAppCommandService;
class SettingsSplitter;
struct AppViewContext;

/**
 * @brief The DtoWaveformView class represents a main view for editing multiple waveforms.
 *
 * Belongs directly to MainWIndow (the second main view of the main toolbar). Contains list with
 * waveforms on the left, waveform editor panel at the center, and property editors on the right.
 */
class DtoWaveformView : public QWidget
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * The services of the context must outlive the view.
   */
  explicit DtoWaveformView(const AppViewContext& context, QWidget* parent_widget = nullptr);
  ~DtoWaveformView() override;

  DtoWaveformView(const DtoWaveformView&) = delete;
  DtoWaveformView& operator=(const DtoWaveformView&) = delete;
  DtoWaveformView(DtoWaveformView&&) = delete;
  DtoWaveformView& operator=(DtoWaveformView&&) = delete;

  /**
   * @brief Perform widgets setup on new project creation or project load from disk.
   */
  void SetWaveformModel(WaveformModel* model);

  /**
   * @brief Set waveform to show in waveform and property editors.
   */
  void SetLineSeriesItem(mvvm::LineSeriesItem* line_series_item);

private:
  void SetupWidgetActions();

  IAppCommandService& m_command_service;
  SettingsSplitter* m_splitter{nullptr};

  QAction* m_toggle_left_sidebar{nullptr};
  QAction* m_toggle_right_sidebar{nullptr};

  DtoWaveformListPanel* m_list_panel{nullptr};
  DtoWaveformEditorPanel* m_editor_panel{nullptr};
  DtoWaveformPropertyPanel* m_property_panel{nullptr};

  WaveformModel* m_model{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_VIEW_H_
