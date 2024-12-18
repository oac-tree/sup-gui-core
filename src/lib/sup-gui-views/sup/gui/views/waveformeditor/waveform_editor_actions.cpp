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

#include <sup/gui/components/component_types.h>
#include <sup/gui/plotting/waveform_editor_action_handler.h>
#include <sup/gui/style/style_helper.h>
#include <sup/gui/widgets/action_menu.h>

#include <mvvm/plotting/plot_types.h>
#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QActionGroup>
#include <QButtonGroup>
#include <QMenu>
#include <QToolButton>
#include <QWidgetAction>

namespace sup::gui
{

WaveformEditorActions::WaveformEditorActions(WaveformEditorActionHandler* action_handler,
                                             QObject* parent)
    : QObject(parent)
    , m_pointer_button_group(new QButtonGroup(this))
    , m_pointer_button(new QToolButton)
    , m_pointer_action(new QWidgetAction(this))
    , m_pan_button(new QToolButton)
    , m_pan_action(new QWidgetAction(this))
    , m_more_settings_menu(CreateMoreSettingsMenu())
    , m_action_handler(action_handler)
{
  SetupCanvasActions();
  SetupTableActions();
}

QList<QAction*> WaveformEditorActions::GetActions(const std::vector<ActionKey>& action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

void WaveformEditorActions::SetPointerButtonGroup(int button_id)
{
  m_pointer_button_group->button(button_id)->setChecked(true);
}

void WaveformEditorActions::SetupCanvasActions()
{
  m_pointer_button->setText("Select");
  m_pointer_button->setIcon(utils::FindIcon("arrow-top-left"));
  m_pointer_button->setToolTip("Scene in edit mode");
  m_pointer_button->setCheckable(true);
  m_pointer_button->setChecked(true);
  m_pointer_button->setToolButtonStyle(Qt::ToolButtonFollowStyle);
  m_pointer_action->setDefaultWidget(m_pointer_button);
  m_action_map.Add(ActionKey::kPointer, m_pointer_action);

  m_pan_button->setText("Pan");
  m_pan_button->setIcon(utils::FindIcon("hand-back-right-outline"));
  m_pan_button->setToolTip("Scene in pan mode (space)");
  m_pan_button->setCheckable(true);
  m_pan_button->setToolButtonStyle(Qt::ToolButtonFollowStyle);
  m_pan_action->setText("Select");
  m_pan_action->setDefaultWidget(m_pan_button);
  m_action_map.Add(ActionKey::kPan, m_pan_action);

  m_pointer_button_group->addButton(m_pointer_button,
                                    static_cast<int>(mvvm::CanvasOperationMode::kSelection));
  m_pointer_button_group->addButton(m_pan_button,
                                    static_cast<int>(mvvm::CanvasOperationMode::kPan));
  connect(m_pointer_button_group, &QButtonGroup::idClicked, this,
          &WaveformEditorActions::ChangeOperationModeRequest);

  m_zoom_in_action = new QAction("Zoom In", this);
  m_zoom_in_action->setIcon(utils::FindIcon("magnify-plus-outline"));
  m_zoom_in_action->setToolTip("Zoom in");
  connect(m_zoom_in_action, &QAction::triggered, this, &WaveformEditorActions::ZoomInRequest);
  m_action_map.Add(ActionKey::kZoomIn, m_zoom_in_action);

  m_zoom_out_action = new QAction("Zoom Out", this);
  m_zoom_out_action->setIcon(utils::FindIcon("magnify-minus-outline"));
  m_zoom_out_action->setToolTip("Zoom out");
  connect(m_zoom_out_action, &QAction::triggered, this, &WaveformEditorActions::ZoomOutRequest);
  m_action_map.Add(ActionKey::kZoomOut, m_zoom_out_action);

  m_center_canvas_action = new QAction("Center", this);
  m_center_canvas_action->setIcon(utils::FindIcon("camera-metering-center"));
  m_center_canvas_action->setToolTip("Reset viewport to default axes range");
  connect(m_center_canvas_action, &QAction::triggered, this,
          &WaveformEditorActions::SetViewportToContentRequest);
  m_action_map.Add(ActionKey::kCenterCanvas, m_center_canvas_action);

  m_more_settings_action = new ActionMenu("Other", this);
  m_more_settings_action->setToolTip("More settings");
  m_more_settings_action->setIcon(utils::FindIcon("menu"));
  m_more_settings_action->setMenu(m_more_settings_menu.get());
  m_action_map.Add(ActionKey::kMoreSettings, m_more_settings_action);
}

void WaveformEditorActions::SetupTableActions()
{
  m_add_column_before = new QAction("Add before", this);
  m_add_column_before->setIcon(utils::FindIcon("table-column-plus-before"));
  m_add_column_before->setText("Add before");
  m_add_column_before->setToolTip("Insert point before selected column");
  connect(m_add_column_before, &QAction::triggered, this,
          [this]() { m_action_handler->OnAddColumnBeforeRequest(); });
  m_action_map.Add(ActionKey::kAddColumnBefore, m_add_column_before);

  m_add_column_after = new QAction("Add after", this);
  m_add_column_after->setIcon(utils::FindIcon("table-column-plus-after"));
  m_add_column_after->setText("Add after");
  m_add_column_after->setToolTip("Insert point after selected column");
  connect(m_add_column_after, &QAction::triggered, this,
          [this]() { m_action_handler->OnAddColumnAfterRequest(); });
  m_action_map.Add(ActionKey::kAddColumnAfter, m_add_column_after);

  m_remove_column = new QAction("Remove", this);
  m_remove_column->setIcon(utils::FindIcon("table-column-remove"));
  m_remove_column->setToolTip("Remove selected column");
  connect(m_remove_column, &QAction::triggered, this,
          [this]() { m_action_handler->OnRemoveColumnRequest(); });
  m_action_map.Add(ActionKey::kRemoveColumn, m_remove_column);
}

std::unique_ptr<QMenu> WaveformEditorActions::CreateMoreSettingsMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto action_group = new QActionGroup(this);

  auto show_all = new QAction("Show all waveforms", this);
  show_all->setToolTip("Show all waveforms on one canvas");
  show_all->setActionGroup(action_group);
  show_all->setCheckable(true);
  show_all->setChecked(true);
  connect(
      show_all, &QAction::triggered, this,
      [this]() {
        emit ChangeWaveformDisplayModeRequest(static_cast<int>(WaveformDisplayMode::kDisplayAll));
      });

  auto show_selected = new QAction("Show selected waveform ", this);
  show_selected->setToolTip("Show only active waveform (i.e. the one selected in the list)");
  show_selected->setActionGroup(action_group);
  show_selected->setCheckable(true);
  connect(show_selected, &QAction::triggered, this,
          [this]()
          {
            emit ChangeWaveformDisplayModeRequest(
                static_cast<int>(WaveformDisplayMode::kDisplaySelected));
          });

  result->addAction(show_all);
  result->addAction(show_selected);

  return result;
}

}  // namespace sup::gui
