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

#include "scalar_quick_toolbar.h"

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/style/style_helper.h>

#include <QToolButton>

namespace sup::gui
{

ScalarQuickToolbar::ScalarQuickToolbar(QWidget* parent_widget) : QToolBar(parent_widget)
{
  // setOrientation(Qt::Vertical);
  setToolButtonStyle(Qt::ToolButtonTextOnly);
  // setIconSize(utils::ToolBarIconSize());

  // setStyleSheet(
  //     "QToolBar { padding-bottom: 0px; margin-bottom: 0px; } QToolButton { padding: 0px; margin:
  //     " "0px; }");
  setStyleSheet("QToolBar { padding-bottom: 0px; margin-bottom: 0px; }");

  PopulateAddScalarButtons();
}

ScalarQuickToolbar::~ScalarQuickToolbar() = default;

void ScalarQuickToolbar::PopulateAddScalarButtons()
{
  for (const auto& scalar_type : GetScalarTypeNames())
  {
    auto button = new QToolButton(this);
    button->setText(QString::fromStdString(scalar_type));
    addWidget(button);
  }
}

}  // namespace sup::gui
