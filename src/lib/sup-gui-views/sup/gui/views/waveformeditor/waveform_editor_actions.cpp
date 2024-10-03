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

#include "waveform_editor_actions.h"

#include <sup/gui/plotting/waveform_editor_action_handler.h>
#include <sup/gui/widgets/style_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QAction>

namespace sup::gui
{

WaveformEditorActions::WaveformEditorActions(WaveformEditorActionHandler* action_handler,
                                             QObject* parent)
    : QObject(parent), m_action_handler(action_handler)
{
  SetupActions();
}

QList<QAction*> WaveformEditorActions::GetActions(const std::vector<ActionKey>& action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

void WaveformEditorActions::SetupActions()
{
  m_add_column_before = new QAction("Add before", this);
  m_add_column_before->setIcon(sup::gui::utils::GetIcon("table-column-plus-before.svg"));
  m_add_column_before->setText("Add before");
  m_add_column_before->setToolTip("Insert point before selected column");
  connect(m_add_column_before, &QAction::triggered, this,
          [this]() { m_action_handler->OnAddColumnBeforeRequest(); });
  m_action_map.Add(ActionKey::kAddColumnBefore, m_add_column_before);

  m_add_column_after = new QAction("Add after", this);
  m_add_column_after->setIcon(sup::gui::utils::GetIcon("table-column-plus-after.svg"));
  m_add_column_after->setText("Add after");
  m_add_column_after->setToolTip("Insert point after selected column");
  connect(m_add_column_after, &QAction::triggered, this,
          [this]() { m_action_handler->OnAddColumnAfterRequest(); });
  m_action_map.Add(ActionKey::kAddColumnAfter, m_add_column_after);

  m_remove_column = new QAction("Remove", this);
  m_remove_column->setIcon(sup::gui::utils::GetIcon("table-column-remove.svg"));
  m_remove_column->setToolTip("Remove selected column");
  connect(m_remove_column, &QAction::triggered, this,
          [this]() { m_action_handler->OnRemoveColumnRequest(); });
  m_action_map.Add(ActionKey::kRemoveColumn, m_remove_column);

  m_zoom_in = new QAction("Zoom In", this);
  m_zoom_in->setIcon(sup::gui::utils::GetIcon("magnify-plus-outline.svg"));
  m_zoom_in->setToolTip("Zoom in");
  connect(m_zoom_in, &QAction::triggered, this, &WaveformEditorActions::ZoomInRequest);
  m_action_map.Add(ActionKey::kZoomIn, m_zoom_in);

  m_zoom_out = new QAction("Zoom Out", this);
  m_zoom_out->setIcon(sup::gui::utils::GetIcon("magnify-minus-outline.svg"));
  m_zoom_out->setToolTip("Zoom out");
  connect(m_zoom_out, &QAction::triggered, this, &WaveformEditorActions::ZoomOutRequest);
  m_action_map.Add(ActionKey::kZoomOut, m_zoom_out);

  m_center_canvas = new QAction("Center", this);
  m_center_canvas->setIcon(sup::gui::utils::GetIcon("camera-metering-center.svg"));
  m_center_canvas->setToolTip("Reset viewport to default axes range");
  connect(m_center_canvas, &QAction::triggered, this,
          &WaveformEditorActions::SetViewportToContentRequest);
  m_action_map.Add(ActionKey::kCenterCanvas, m_center_canvas);
}

}  // namespace sup::gui
