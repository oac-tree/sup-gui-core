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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_LIST_PANEL_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_LIST_PANEL_H_

#include <sup/gui/components/dto_waveform_editor_context.h>

#include <QWidget>
#include <memory>

class QListView;

namespace mvvm
{
class ItemViewComponentProvider;
class LineSeriesItem;
class ChartViewportItem;
}  // namespace mvvm

namespace sup::gui
{

class ItemStackWidget;
class DtoWaveformActionHandler;
class DtoWaveformActions;
class WaveformModel;

/**
 * @brief The DtoWaveformListPanel class represents a vertical panel with the list of available
 * waveforms.
 *
 * Located on the left of DtoWaveformView.
 */
class DtoWaveformListPanel : public QWidget
{
  Q_OBJECT
public:
  explicit DtoWaveformListPanel(QWidget* parent_widget = nullptr);
  ~DtoWaveformListPanel() override;

  /**
   * @brief Sets main viewport with waveforms.
   */
  void SetViewport(mvvm::ChartViewportItem* viewport);

  mvvm::LineSeriesItem* GetSelectedWaveform();

  void SetSelectedWaveform(mvvm::LineSeriesItem* waveform);

signals:
  void WaveformSelected(mvvm::LineSeriesItem* waveform);

private:
  /**
   * @brief Create context describing current status of the widget.
   */
  DtoWaveformEditorContext CreateContext();

  WaveformModel* GetWaveformModel();

  DtoWaveformActionHandler* m_action_handler{nullptr};
  DtoWaveformActions* m_actions{nullptr};

  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
  QListView* m_list_view{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;

  mvvm::ChartViewportItem* m_chart_viewport{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_LIST_PANEL_H_
