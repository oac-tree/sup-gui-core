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

#include "settings_editor_dialog.h"

#include "settings_editor.h"
#include "settings_helper.h"

#include <sup/gui/widgets/dialog_helper.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QKeyEvent>
#include <QLabel>
#include <QSettings>
#include <QVBoxLayout>

namespace sup::gui
{

namespace
{

const QString kGroupName = "SettingsEditorDialog";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";

}  // namespace

SettingsEditorDialog::SettingsEditorDialog(QWidget* parent_widget)
    : QDialog(parent_widget), m_label(new QLabel), m_settings_editor(new SettingsEditor)
{
  setWindowTitle("Application Settings");

  m_label->setText("Changes in settings will come into effect at the next application start");
  m_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  m_label->setFixedHeight(mvvm::utils::UnitSize(2));

  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_settings_editor);
  layout->addWidget(m_label);
  // layout->addSpacing(mvvm::utils::UnitSize(0.5));
  layout->addLayout(sup::gui::CreateButtonLayout(this, "Save settings", "Cancel").release());

  ReadSettings();
}

SettingsEditorDialog::~SettingsEditorDialog()
{
  WriteSettings();
}

void SettingsEditorDialog::SetInitialValues(const mvvm::ISessionModel &model)
{
  m_settings_editor->SetInitialValues(model);
}

const SettingsModel* SettingsEditorDialog::GetResult() const
{
  return m_settings_editor->GetResult();
}

void SettingsEditorDialog::keyPressEvent(QKeyEvent* event)
{
  // Prevent dialog from closing on press-enter-key event. This seems to be the only reliable way
  // to avoid forwarding such events from QLineEdit to "OK" QPushButton.

  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    return;
  }

  QDialog::keyPressEvent(event);
}

void SettingsEditorDialog::ReadSettings()
{
  const QSettings settings;
  const auto default_size = QSize(mvvm::utils::UnitSize(30), mvvm::utils::UnitSize(30));
  resize(settings.value(kWindowSizeSettingName, default_size).toSize());
}

void SettingsEditorDialog::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWindowSizeSettingName, size());
}

}  // namespace sup::gui
