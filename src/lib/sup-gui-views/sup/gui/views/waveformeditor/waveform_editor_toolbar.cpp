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

#include "waveform_editor_toolbar.h"

#include <sup/gui/widgets/style_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QToolButton>

namespace sup::gui
{

WaveformEditorToolBar::WaveformEditorToolBar(QWidget *parent)
    : QToolBar(parent)
    , m_add_column_before(new QToolButton)
    , m_add_column_after(new QToolButton)
    , m_remove_column(new QToolButton)
    , m_zoom_in(new QToolButton)
    , m_zoom_out(new QToolButton)
    , m_center_canvas(new QToolButton)
{
  const int width = mvvm::utils::UnitSize(2.2);
  const QSize size{width, width};

  setIconSize(size);
  setOrientation(Qt::Vertical);
  setStyleSheet(
      "QToolBar { padding: 0px; margin: 0px; } QToolButton { padding: 0px; margin: 0px; }");

  m_center_canvas->setIcon(sup::gui::utils::GetIcon("camera-metering-center.svg"));
  m_center_canvas->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_center_canvas->setToolTip("Reset viewport to default axes range");
  connect(m_center_canvas, &QToolButton::clicked, this,
          &WaveformEditorToolBar::SetViewportToContentRequest);
  addWidget(m_center_canvas);

  m_zoom_in->setIcon(sup::gui::utils::GetIcon("magnify-plus-outline.svg"));
  m_zoom_in->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_zoom_in->setToolTip("Zoom in");
  connect(m_zoom_in, &QToolButton::clicked, this, &WaveformEditorToolBar::ZoomInRequest);
  addWidget(m_zoom_in);

  m_zoom_out->setIcon(sup::gui::utils::GetIcon("magnify-minus-outline.svg"));
  m_zoom_out->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_zoom_out->setToolTip("Zoom out");
  connect(m_zoom_out, &QToolButton::clicked, this, &WaveformEditorToolBar::ZoomOutRequest);
  addWidget(m_zoom_out);

  addSeparator();

  m_add_column_before->setIcon(sup::gui::utils::GetIcon("table-column-plus-before-v2.svg"));
  m_add_column_before->setIconSize(size);
  m_add_column_before->setText("Add before");
  m_add_column_before->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_add_column_before->setToolTip("Insert point before selected column");
  connect(m_add_column_before, &QToolButton::clicked, this,
          &WaveformEditorToolBar::AddColumnBeforeRequest);
  addWidget(m_add_column_before);

  m_add_column_after->setIcon(sup::gui::utils::GetIcon("table-column-plus-after-v2.svg"));
  m_add_column_after->setText("Add after");
  m_add_column_after->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_add_column_after->setToolTip("Insert point after selected column");
  connect(m_add_column_after, &QToolButton::clicked, this,
          &WaveformEditorToolBar::AddColumnAfterRequest);
  addWidget(m_add_column_after);

  m_remove_column->setIcon(sup::gui::utils::GetIcon("table-column-remove-v2.svg"));
  m_remove_column->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_remove_column->setToolTip("Remove selected column");
  connect(m_remove_column, &QToolButton::clicked, this,
          &WaveformEditorToolBar::RemoveColumnRequest);
  addWidget(m_remove_column);
}

}  // namespace sup::gui
