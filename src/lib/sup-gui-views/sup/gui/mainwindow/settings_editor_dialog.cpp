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

#include "settings_editor_dialog.h"

#include "settings_editor.h"
#include "settings_helper.h"

#include <sup/gui/settings/i_settings_manager.h>
#include <sup/gui/widgets/dialog_helper.h>
#include <sup/gui/widgets/window_info.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/style/mvvm_style_helper.h>

#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>

namespace sup::gui
{

namespace
{

//! Settings group of the dialog geometry.
constexpr auto kWindowSettingsGroup = "SettingsEditorDialog";

//! Returns the default dialog size.
QSize GetDefaultSize()
{
  return {mvvm::style::UnitSize(30), mvvm::style::UnitSize(30)};
}

}  // namespace

SettingsEditorDialog::SettingsEditorDialog(const mvvm::ISessionModel& model,
                                           ISettingsManager& settings_manager,
                                           QWidget* parent_widget)
    : QDialog(parent_widget)
    , m_label(new QLabel)
    , m_settings_editor(new SettingsEditor(model))
    , m_settings_manager(settings_manager)
{
  setWindowTitle("Application Settings");

  m_label->setText("Changes in settings will come into effect at the next application start");
  m_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  m_label->setFixedHeight(mvvm::style::UnitSize(2));

  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_settings_editor);
  layout->addWidget(m_label);
  layout->addLayout(CreateButtonLayout(this, "Save settings", "Cancel").release());

  const auto info =
      m_settings_manager.Get<WindowInfo>(kWindowSettingsGroup, SettingsScope::kApplication);
  if (!restoreGeometry(info.geometry))
  {
    resize(GetDefaultSize());
  }
}

SettingsEditorDialog::~SettingsEditorDialog() = default;

void SettingsEditorDialog::PropagateSettingsToModel(mvvm::ISessionModel& model)
{
  m_settings_editor->PropagateSettingsToModel(model);
}

void SettingsEditorDialog::RegisterWidget(const std::string& key,
                                          const widget_factory_func_t& factory_func)
{
  m_settings_editor->RegisterWidget(key, factory_func);
}

void SettingsEditorDialog::done(int result)
{
  m_settings_manager.Set(kWindowSettingsGroup, WindowInfo{saveGeometry()},
                         SettingsScope::kApplication);
  QDialog::done(result);
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

}  // namespace sup::gui
