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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "common_settings_editor.h"

#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/views/property_flat_view.h>
#include <mvvm/widgets/widget_utils.h>

#include <QVBoxLayout>

namespace sup::gui
{

CommonSettingsEditor::CommonSettingsEditor(QWidget *parent_widget)
    : SessionItemWidget(parent_widget)
    , m_settings_view(new mvvm::PropertyFlatView)
    , m_property_view_model(std::make_unique<mvvm::PropertyViewModel>(nullptr))
{
  auto layout = new QVBoxLayout(this);

  layout->addWidget(m_settings_view);

  m_settings_view->layout()->setContentsMargins(mvvm::utils::UnitSize(1), 0, 0, 0);
  m_settings_view->SetViewModel(m_property_view_model.get());
}

CommonSettingsEditor::~CommonSettingsEditor() = default;

void CommonSettingsEditor::SetItem(mvvm::SessionItem *item)
{
  m_property_view_model->SetRootSessionItem(item);
}

}  // namespace sup::gui
