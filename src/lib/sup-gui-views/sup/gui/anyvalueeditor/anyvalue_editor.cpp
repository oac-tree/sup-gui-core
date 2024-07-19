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

#include "anyvalue_editor_dialog.h"
#include "anyvalue_editor_widget.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/waveformeditor/anyvalue_waveform_editor.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/item_utils.h>

#include <QVBoxLayout>

namespace
{

std::unique_ptr<sup::gui::AnyValueEditorDialog> CreateWaveformEditorDialog(
    const sup::gui::AnyValueItem *item, QWidget *parent)
{
  auto editor = std::make_unique<sup::gui::AnyValueWaveformEditor>();
  editor->SetInitialValue(item);
  auto result = std::make_unique<sup::gui::AnyValueEditorDialog>(std::move(editor), parent);
  result->setWindowTitle("Import Waveform");
  return result;
}

}  // namespace

namespace sup::gui
{

AnyValueEditor::AnyValueEditor(QWidget *parent)
    : AbstractAnyValueEditor(parent)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_editor_widget(new AnyValueEditorWidget(m_model.get()))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_editor_widget);
}

void AnyValueEditor::SetInitialValue(const AnyValueItem *item)
{
  m_editor_widget->SetInitialValue(*item);
}

std::unique_ptr<AnyValueItem> AnyValueEditor::GetResult()
{
  if (auto top_item = GetTopItem(); top_item)
  {
    return mvvm::utils::CloneItem(*top_item);
  }

  return {};
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

void AnyValueEditor::OnImportWaveformRequest()
{
  auto dialog = CreateWaveformEditorDialog(GetTopItem(), this);
  if (dialog->exec() == QDialog::Accepted)
  {
    auto result = dialog->GetResult();
    m_editor_widget->SetInitialValue(*result.get());
  }
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
