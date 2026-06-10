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

#include "splittable_editor_widget.h"

#include "splittable_combo_panel.h"

#include <QVBoxLayout>

namespace sup::gui
{

SplittableEditorWidget::SplittableEditorWidget(QWidget* parent_widget)
    : QWidget(parent_widget), m_config_combo_panel(new SplittableComboPanel)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_config_combo_panel);
}

void SplittableEditorWidget::SetModel(ConfigurationModel* model)
{
  m_config_combo_panel->SetModel(model);
}

}  // namespace sup::gui
