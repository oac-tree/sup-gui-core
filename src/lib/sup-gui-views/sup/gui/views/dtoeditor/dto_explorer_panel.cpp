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

#include "dto_explorer_panel.h"

#include <sup/gui/widgets/file_tree_view.h>
#include <sup/gui/widgets/item_stack_widget.h>

#include <QVBoxLayout>

namespace sup::gui
{

namespace
{
constexpr auto kFileTreeSettingsGroup = "FileTreeView";

}

DtoExplorerPanel::DtoExplorerPanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_file_tree_view(new sup::gui::FileTreeView(kFileTreeSettingsGroup))
    , m_stack_widget(new sup::gui::ItemStackWidget)
{
  m_file_tree_view->setWindowTitle("Explorer View");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_stack_widget->AddWidget(m_file_tree_view, m_file_tree_view->actions());
  layout->addWidget(m_stack_widget);
}

}  // namespace sup::gui
