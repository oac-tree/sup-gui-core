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

#include "dto_compare_view.h"

#include "dto_explorer_panel.h"
#include "splittable_config_editor.h"

#include <sup/gui/widgets/custom_splitter.h>

#include <QVBoxLayout>

namespace sup::gui
{

namespace
{

constexpr auto kSplitterSettingName("DtoCompareView/splitter");

}

DtoCompareView::DtoCompareView(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_splitter(new sup::gui::CustomSplitter(kSplitterSettingName))
    , m_explorer_panel(new DtoExplorerPanel)
    , m_splittable_config_editor(new SplittableConfigEditor)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);
  layout->setSpacing(0);
  layout->addWidget(m_splitter);

  m_splitter->addWidget(m_explorer_panel);
  m_splitter->addWidget(m_splittable_config_editor);

  ReadSettings();
}

DtoCompareView::~DtoCompareView()
{
  WriteSettings();
}

void DtoCompareView::ReadSettings()
{
  m_splitter->ReadSettings();
}

void DtoCompareView::WriteSettings()
{
  m_splitter->WriteSettings();
}

}  // namespace sup::gui
