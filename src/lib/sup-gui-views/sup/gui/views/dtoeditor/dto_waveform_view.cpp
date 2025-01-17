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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "dto_waveform_view.h"

#include "dto_waveform_editor_panel.h"
#include "dto_waveform_list_panel.h"
#include "dto_waveform_property_panel.h"

#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>
#include <sup/gui/model/waveform_model.h>
#include <sup/gui/style/style_helper.h>

#include <mvvm/standarditems/line_series_item.h>

#include <QSplitter>
#include <QVBoxLayout>

namespace sup::gui
{

DtoWaveformView::DtoWaveformView(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_splitter(new QSplitter)
    , m_list_panel(new DtoWaveformListPanel)
    , m_editor_panel(new DtoWaveformEditorPanel)
    , m_property_panel(new DtoWaveformPropertyPanel)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_splitter->addWidget(m_list_panel);
  m_splitter->addWidget(m_editor_panel);
  m_splitter->addWidget(m_property_panel);
  m_splitter->setSizes({200, 300, 200});

  layout->addWidget(m_splitter);

  SetupWidgetActions();

  connect(m_list_panel, &DtoWaveformListPanel::WaveformSelected, this,
          &DtoWaveformView::SetLineSeriesItem);
}

DtoWaveformView::~DtoWaveformView() = default;

void DtoWaveformView::SetLineSeriesItem(mvvm::LineSeriesItem* line_series_item)
{
  m_editor_panel->SetLineSeriesItem(line_series_item);
  m_property_panel->SetLineSeriesItem(line_series_item);
}

void DtoWaveformView::SetupWidgetActions()
{
  m_toggle_left_sidebar = new QAction("Show/hide left panel", this);
  m_toggle_left_sidebar->setToolTip("Show/hide left panel");
  m_toggle_left_sidebar->setIcon(utils::FindIcon("dock-left"));
  connect(m_toggle_left_sidebar, &QAction::triggered, this,
          [this](auto) { m_list_panel->setVisible(!m_list_panel->isVisible()); });

  m_toggle_right_sidebar = new QAction("Show/hide right panel", this);
  m_toggle_right_sidebar->setToolTip("Show/hide right panel");
  m_toggle_right_sidebar->setIcon(utils::FindIcon("dock-right"));
  connect(m_toggle_right_sidebar, &QAction::triggered, this,
          [this](auto) { m_property_panel->setVisible(!m_property_panel->isVisible()); });

  const auto context = sup::gui::AppRegisterWidgetUniqueId(this);
  AppAddActionToCommand(m_toggle_left_sidebar, constants::kToggleLeftPanelCommandId, context);
  AppAddActionToCommand(m_toggle_right_sidebar, constants::kToggleRightPanelCommandId, context);
}

void DtoWaveformView::SetWaveformModel(WaveformModel* model)
{
  m_list_panel->SetViewport(model->GetViewPort());
  m_editor_panel->SetViewport(model->GetViewPort());
}

}  // namespace sup::gui
