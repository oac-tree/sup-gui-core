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

#include "scalar_quick_toolbar.h"

#include <sup/gui/components/anyvalue_editor_action_handler.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/style/style_helper.h>
#include <sup/gui/widgets/flow_layout.h>

#include <mvvm/widgets/appearance_helper.h>

#include <QLayout>
#include <QToolButton>

namespace sup::gui
{

ScalarQuickToolbar::ScalarQuickToolbar(AnyValueEditorActionHandler* action_handler,
                                       QWidget* parent_widget)
    : QWidget(parent_widget), m_action_handler(action_handler)
{
  auto layout = new FlowLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  PopulateAddScalarButtons(layout);
}

ScalarQuickToolbar::~ScalarQuickToolbar() = default;

void ScalarQuickToolbar::PopulateAddScalarButtons(QLayout* layout)
{
  for (const auto& scalar_type : GetScalarTypeNames())
  {
    auto button = new QToolButton(this);
    button->setText(QString::fromStdString(scalar_type));
    button->setStyleSheet(mvvm::GetToolBarButtonStyleString());
    button->setToolButtonStyle(Qt::ToolButtonTextOnly);
    button->setFocusPolicy(Qt::NoFocus);
    button->setToolTip(
        "Add scalar of given type.\n\n"
        "If possible, it will be added after the selected item; "
        "otherwise, \nit will be added as a child of the selected item.");
    connect(button, &QToolButton::clicked, this,
            [this, scalar_type]() { InsertScalar(scalar_type); });
    layout->addWidget(button);
  }
}

void ScalarQuickToolbar::InsertScalar(const std::string& scalar_type)
{
  if (m_action_handler->CanInsertAfter(scalar_type))
  {
    m_action_handler->InsertAnyValueItemAfter(scalar_type);
  }

  else if (m_action_handler->CanInsertInto(scalar_type))
  {
    m_action_handler->InsertAnyValueItemInto(scalar_type);
  }
  else
  {
    // nothing to do
  }
}

}  // namespace sup::gui
