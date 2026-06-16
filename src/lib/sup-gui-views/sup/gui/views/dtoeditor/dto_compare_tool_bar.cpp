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

#include <sup/gui/style/style_helper.h>

#include <QCheckBox>
#include <QLabel>

namespace sup::gui
{

namespace
{
constexpr auto kSyncToolTip =
    "Synchronize panel behaviour\nFor multiple panels their behavior will be synchronised, where "
    "possible (e.g. synchronious scroll bar move, reaction on filter, etc), if Sync is cheked";
}

DtoCompareToolBar::DtoCompareToolBar(QWidget* parent_widget)
    : QToolBar(parent_widget), m_sync_check_box(new QCheckBox)
{
  setIconSize(utils::ToolBarIconSize());

  m_sync_check_box->setToolTip(kSyncToolTip);
  addWidget(m_sync_check_box);

  auto sync_label = new QLabel("Sync");
  sync_label->setToolTip(kSyncToolTip);
  addWidget(sync_label);
}

}  // namespace sup::gui
