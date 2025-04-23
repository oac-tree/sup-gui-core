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

#include "anyvalue_editor_dialog.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/views/anyvalueeditor/abstract_anyvalue_editor.h>
#include <sup/gui/widgets/dialog_helper.h>

#include <QKeyEvent>
#include <QLabel>
#include <QSettings>
#include <QVBoxLayout>

namespace
{

QString GetDialogSizeSettingName(const QString& editor_name)
{
  return QString("AnyValueEditorDialog") + "/" + editor_name + "window_size";
}

}  // namespace

namespace sup::gui
{

AnyValueEditorDialog::AnyValueEditorDialog(std::unique_ptr<sup::gui::AbstractAnyValueEditor> editor,
                                           QWidget* parent)
    : QDialog(parent), m_anyvalue_editor(editor.release())
{
  setWindowTitle("Input request");
  ReadSettings();

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_anyvalue_editor);
  layout->addLayout(CreateButtonLayout(this, "Set AnyValue", "Cancel").release());
}

AnyValueEditorDialog::~AnyValueEditorDialog()
{
  WriteSettings();
}

void AnyValueEditorDialog::SetInitialValue(const sup::gui::AnyValueItem* item)
{
  m_anyvalue_editor->SetInitialValue(item);
}

std::unique_ptr<sup::gui::AnyValueItem> AnyValueEditorDialog::GetResult()
{
  return m_anyvalue_editor->GetResult();
}

void AnyValueEditorDialog::SetDescription(const QString& description)
{
  m_anyvalue_editor->SetDescription(description);
}

void AnyValueEditorDialog::keyPressEvent(QKeyEvent* event)
{
  // Prevent dialog from closing on enter-key-event. This is necessary since some of underlying
  // widgets might have own ideas and might forward this key event back.

  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    return;
  }

  QDialog::keyPressEvent(event);
}

void AnyValueEditorDialog::ReadSettings()
{
  const QSettings settings;
  const auto setting_name = GetDialogSizeSettingName(m_anyvalue_editor->windowTitle());
  if (settings.contains(setting_name))
  {
    resize(settings.value(setting_name).toSize());
  }
}

void AnyValueEditorDialog::WriteSettings()
{
  QSettings settings;
  settings.setValue(GetDialogSizeSettingName(m_anyvalue_editor->windowTitle()), size());
}

}  // namespace sup::gui
