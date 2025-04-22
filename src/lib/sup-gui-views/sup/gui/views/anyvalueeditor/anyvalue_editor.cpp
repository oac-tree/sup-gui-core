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

#include "anyvalue_editor.h"

#include "anyvalue_editor_widget.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>

#include <QVBoxLayout>

namespace
{

const bool kEnableUndo = true;
const std::size_t kUndoLimit = 100;

}  // namespace

namespace sup::gui
{

AnyValueEditor::AnyValueEditor(QWidget *parent_widget)
    : AbstractAnyValueEditor(parent_widget)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_editor_widget(new AnyValueEditorWidget)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_editor_widget);

  m_model->SetUndoEnabled(kEnableUndo, kUndoLimit);
  m_editor_widget->SetAnyValueItemContainer(m_model->GetRootItem());
}

AnyValueEditor::~AnyValueEditor() = default;

void AnyValueEditor::SetInitialValue(const AnyValueItem *item)
{
  m_editor_widget->SetInitialValue(*item);
}

std::unique_ptr<AnyValueItem> AnyValueEditor::GetResult()
{
  return m_editor_widget->GetResult();
}

AnyValueItem *AnyValueEditor::GetTopItem()
{
  return m_editor_widget->GetTopItem();
}

}  // namespace sup::gui
