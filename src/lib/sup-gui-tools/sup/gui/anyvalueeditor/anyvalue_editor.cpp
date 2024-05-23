/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalue_editor.h"

#include "anyvalue_editor_widget.h"

#include <mvvm/model/application_model.h>
#include <mvvm/signals/model_listener.h>

#include <QVBoxLayout>

namespace sup::gui
{

AnyValueEditor::AnyValueEditor(QWidget *parent)
    : QWidget(parent)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_listener(std::make_unique<listener_t>(m_model.get()))
    , m_editor_widget(new AnyValueEditorWidget(m_model.get()))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_editor_widget);

  m_listener->Connect<mvvm::ModelResetEvent>([this](auto) { OnProjectLoad(); });
}

void AnyValueEditor::SetInitialValue(const AnyValueItem &item)
{
  m_editor_widget->SetInitialValue(item);
}

AnyValueItem *AnyValueEditor::GetTopItem()
{
  return m_editor_widget->GetTopItem();
}

mvvm::ApplicationModel *AnyValueEditor::GetModel() const
{
  return m_model.get();
}

void AnyValueEditor::OnImportFromFileRequest()
{
  m_editor_widget->OnImportFromFileRequest();
}

void AnyValueEditor::OnExportToFileRequest()
{
  m_editor_widget->OnExportToFileRequest();
}

void AnyValueEditor::OnProjectLoad()
{
  m_editor_widget->SetAnyValueItemContainer(m_model->GetRootItem());
}

AnyValueEditor::~AnyValueEditor() = default;

}  // namespace sup::gui
