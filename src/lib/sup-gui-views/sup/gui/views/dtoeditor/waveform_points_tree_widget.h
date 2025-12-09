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

#ifndef SUP_GUI_VIEWS_DTOEDITOR_WAVEFORM_POINTS_TREE_WIDGET_H_
#define SUP_GUI_VIEWS_DTOEDITOR_WAVEFORM_POINTS_TREE_WIDGET_H_

#include <QWidget>

class QTreeView;

namespace mvvm
{
class AllItemsTreeView;
class LineSeriesItem;
class ItemViewComponentProvider;
}  // namespace mvvm

namespace sup::gui
{

/**
 * @brief The tree with waveform points.
 *
 * Embedded in DtoWaveformPropertyPanel, located on the right panel of DtoWaveformView.
 */
class WaveformPointsTreeWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WaveformPointsTreeWidget(QWidget* parent_widget = nullptr);

  /**
   * @brief Set waveform to show in property editor.
   */
  void SetLineSeriesItem(mvvm::LineSeriesItem* line_series_item);

private:
  QTreeView* m_tree_view{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
};

}  // namespace sup::gui

#endif  // SUP_GUI_VIEWS_DTOEDITOR_WAVEFORM_POINTS_TREE_WIDGET_H_
