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

#include "dto_compare_tool_bar.h"

#include <sup/gui/components/component_types.h>
#include <sup/gui/style/style_helper.h>
#include <sup/gui/widgets/action_menu.h>

#include <QCheckBox>
#include <QLabel>
#include <QMenu>
#include <QToolButton>
#include <map>

namespace sup::gui
{

namespace
{
constexpr auto kSyncToolTip =
    "Will synchronize panel behaviour, if checked.\nFor multiple panels their behavior will be "
    "synchronised,\nwhere possible (e.g. synchronious scroll bar move,\nreaction on filter, etc).";

QString GetTextForViewMode(ConfigurationViewMode mode)
{
  static const std::map<ConfigurationViewMode, QString> kModeToText = {
      {ConfigurationViewMode::kTree, "Tree view"},
      {ConfigurationViewMode::kJSON, "JSON view"},
      {ConfigurationViewMode::kFlat, "Flat view"}};
  auto it = kModeToText.find(mode);
  if (it != kModeToText.end())
  {
    return it->second;
  }
  return {"Unknown view"};
}

}  // namespace

DtoCompareToolBar::DtoCompareToolBar(QWidget* parent_widget)
    : QToolBar(parent_widget)
    , m_select_viewer_menu(CreateSetEditorMenu())
    , m_select_viewer_action(new ActionMenu("Set editor", this))
    , m_sync_check_box(new QCheckBox)
    , m_dots_button(new QToolButton)
{
  setIconSize(utils::ToolBarIconSize());
  setToolButtonStyle(Qt::ToolButtonTextOnly);

  // action to select editor for the set
  m_select_viewer_action->setIcon(utils::FindIcon("plus-circle-outline"));
  m_select_viewer_action->setToolTip("Select the viewer to look at the configuration");
  m_select_viewer_action->setMenu(m_select_viewer_menu.get());
  addAction(m_select_viewer_action);

  // sync checkbox
  m_sync_check_box->setToolTip(kSyncToolTip);
  addWidget(m_sync_check_box);

  // sync label
  auto sync_label = new QLabel(" Sync");
  sync_label->setToolTip(kSyncToolTip);
  addWidget(sync_label);

  InsertStrech();

  m_dots_button->setIcon(utils::FindIcon("dots-vertical"));
  m_dots_button->setIconSize(utils::ToolBarIconSize());
  addWidget(m_dots_button);
}

void DtoCompareToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

std::unique_ptr<QMenu> DtoCompareToolBar::CreateSetEditorMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  for (int mode = 0; mode < static_cast<int>(ConfigurationViewMode::kTotalCount); ++mode)
  {
    auto view_mode = static_cast<ConfigurationViewMode>(mode);
    const auto text = GetTextForViewMode(view_mode);
    auto action = result->addAction(text);
    auto on_triggered = [this, mode, text]()
    {
      m_select_viewer_action->setText(text);
      emit configurationViewModeRequest(mode);
    };
    connect(action, &QAction::triggered, this, on_triggered);
  }

  return result;
}

}  // namespace sup::gui
