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

#include "waveform_editor_widget.h"

#include "waveform_editor_actions.h"
#include "waveform_table_widget.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/plotting/waveform_editor_action_handler.h>
#include <sup/gui/widgets/style_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/point_item.h>
#include <mvvm/views/chart_canvas.h>

#include <QSplitter>
#include <QToolBar>
#include <QVBoxLayout>

namespace sup::gui
{

namespace
{

/**
 * @brief Returns list of actions intended for canvas toolbar.
 */
QList<QAction *> GetCanvasToolBarActions(WaveformEditorActions *editor_actions)
{
  using ActionKey = WaveformEditorActions::ActionKey;
  return editor_actions->GetActions({ActionKey::kPointer, ActionKey::kPan, ActionKey::kZoomIn,
                                     ActionKey::kZoomOut, ActionKey::kCenterCanvas});
}

/**
 * @brief Returns list of actions intended for canvas toolbar.
 */
QList<QAction *> GetTableToolBarActions(WaveformEditorActions *editor_actions)
{
  using ActionKey = WaveformEditorActions::ActionKey;
  return editor_actions->GetActions(
      {ActionKey::kAddColumnBefore, ActionKey::kAddColumnAfter, ActionKey::kRemoveColumn});
}

}  // namespace

WaveformEditorWidget::WaveformEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_action_handler(new WaveformEditorActionHandler(CreateActionContext(), this))
    , m_actions(new WaveformEditorActions(m_action_handler, this))
    , m_splitter(new QSplitter)
    , m_chart_canvas(new mvvm::ChartCanvas)
    , m_table_tool_bar(new QToolBar)
    , m_table_widget(new WaveformTableWidget(nullptr))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_splitter->setOrientation(Qt::Vertical);
  m_splitter->addWidget(m_chart_canvas);
  m_splitter->addWidget(CreateTableEnvelopWidget().release());

  layout->addWidget(m_splitter);

  addActions(GetCanvasToolBarActions(m_actions));

  SetupConnections();
}

mvvm::LineSeriesItem *WaveformEditorWidget::GetLineSeriesItem() const
{
  return m_table_widget->GetLineSeriesItem();
}

WaveformEditorWidget::~WaveformEditorWidget() = default;

void WaveformEditorWidget::SetLineSeriesItem(mvvm::LineSeriesItem *line_series_item)
{
  m_table_widget->SetLineSeriesItem(line_series_item);
}

void WaveformEditorWidget::SetViewportItem(mvvm::ChartViewportItem *viewport_item)
{
  m_chart_canvas->SetViewport(viewport_item);
}

mvvm::PointItem *WaveformEditorWidget::GetSelectedPoint() const
{
  return m_table_widget->GetSelectedPoint();
}

void WaveformEditorWidget::ZoomIn()
{
  m_chart_canvas->ZoomIn();
}

void WaveformEditorWidget::ZoomOut()
{
  m_chart_canvas->ZoomOut();
}

void WaveformEditorWidget::SetViewportToContent()
{
  m_chart_canvas->SetViewportToContent();
}

void WaveformEditorWidget::SetSelectedPoint(const mvvm::PointItem *item)
{
  m_table_widget->SetSelectedPoint(item);
}

WaveformEditorContext WaveformEditorWidget::CreateActionContext() const
{
  auto get_current_line_series = [this]() { return GetLineSeriesItem(); };

  auto get_selected_point_callback = [this]() { return GetSelectedPoint(); };
  return {get_current_line_series, get_selected_point_callback};
}

void WaveformEditorWidget::SetupConnections()
{
  connect(m_actions, &WaveformEditorActions::ZoomInRequest, this, &WaveformEditorWidget::ZoomIn);
  connect(m_actions, &WaveformEditorActions::ZoomOutRequest, this, &WaveformEditorWidget::ZoomOut);
  connect(m_actions, &WaveformEditorActions::SetViewportToContentRequest, this,
          &WaveformEditorWidget::SetViewportToContent);

  connect(m_action_handler, &WaveformEditorActionHandler::SelectItemRequest, this,
          [this](auto item) { SetSelectedPoint(dynamic_cast<const mvvm::PointItem *>(item)); });

  // propagate request to change canvas operation mode (select/pan) from toolbar to canvas
  connect(m_actions, &WaveformEditorActions::ChangeOperationModeRequest, m_chart_canvas,
          &mvvm::ChartCanvas::SetOperationMode);

  // propagate notification from canvas to toolbar, that
  connect(m_chart_canvas, &mvvm::ChartCanvas::OperationModeChanged, m_actions,
          &WaveformEditorActions::SetPointerButtonGroup);
}

std::unique_ptr<QWidget> WaveformEditorWidget::CreateTableEnvelopWidget()
{
  auto result = std::make_unique<QWidget>();

  m_table_tool_bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_table_tool_bar->setIconSize(sup::gui::utils::ToolBarIconSize());
  m_table_tool_bar->addActions(GetTableToolBarActions(m_actions));

  auto layout = new QVBoxLayout(result.get());
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_table_tool_bar);
  layout->addWidget(m_table_widget);

  return result;
}

}  // namespace sup::gui
