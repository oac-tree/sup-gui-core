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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_PROPERTY_PANEL_H_
#define SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_PROPERTY_PANEL_H_

#include <QWidget>

namespace mvvm
{
class AllItemsTreeView;
class ISessionModel;
class PropertyTreeView;
class LineSeriesItem;
class PointItem;
}  // namespace mvvm

namespace sup::gui
{

class ItemStackWidget;
class WaveformPointsTreeWidget;

/**
 * @brief The DtoWaveformPropertyPanel class is a vertical panel with waveform tree-like
 * property editors.
 *
 * Located on the right of DtoWaveformformView.
 */
class DtoWaveformPropertyPanel : public QWidget
{
  Q_OBJECT

public:
  explicit DtoWaveformPropertyPanel(QWidget* parent_widget = nullptr);
  ~DtoWaveformPropertyPanel() override;

  DtoWaveformPropertyPanel(const DtoWaveformPropertyPanel&) = delete;
  DtoWaveformPropertyPanel& operator=(const DtoWaveformPropertyPanel&) = delete;
  DtoWaveformPropertyPanel(DtoWaveformPropertyPanel&&) = delete;
  DtoWaveformPropertyPanel& operator=(DtoWaveformPropertyPanel&&) = delete;

  /**
   * @brief Set waveform to show in property editor.
   */
  void SetLineSeriesItem(mvvm::LineSeriesItem* line_series_item);

signals:
  void pointSelected(mvvm::PointItem* point_item);

private:
  ItemStackWidget* m_stack_widget{nullptr};
  mvvm::AllItemsTreeView* m_plot_property_tree{nullptr};
  WaveformPointsTreeWidget* m_waveform_tree{nullptr};
  mvvm::PropertyTreeView* m_viewport_property_tree{nullptr};
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_DTO_WAVEFORM_PROPERTY_PANEL_H_
