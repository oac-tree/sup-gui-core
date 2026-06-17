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
#include <QToolButton>

namespace sup::gui
{

namespace
{
constexpr auto kSyncToolTip =
    "Will synchronize panel behaviour, if checked.\nFor multiple panels their behavior will be "
    "synchronised,\nwhere possible (e.g. synchronious scroll bar move,\nreaction on filter, etc).";

}  // namespace

DtoCompareToolBar::DtoCompareToolBar(QWidget* parent_widget)
    : QToolBar(parent_widget), m_sync_check_box(new QCheckBox), m_dots_button(new QToolButton)
{
  setIconSize(utils::ToolBarIconSize());
  setToolButtonStyle(Qt::ToolButtonTextOnly);

  // sync checkbox
  m_sync_check_box->setToolTip(kSyncToolTip);
  connect(m_sync_check_box, &QCheckBox::toggled, this,
          &DtoCompareToolBar::panelSyncAppearanceFlagChanged);
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

}  // namespace sup::gui
