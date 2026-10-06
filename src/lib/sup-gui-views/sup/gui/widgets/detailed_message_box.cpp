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

#include "detailed_message_box.h"

#include "dialog_helper.h"
#include "window_info.h"

#include <sup/gui/settings/i_settings_manager.h>

#include <mvvm/style/mvvm_style_helper.h>

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QSettings>
#include <QStyle>
#include <QVBoxLayout>

namespace sup::gui
{

namespace
{

QString GetDialogSizeSettingName(const QString& setting_key)
{
  return setting_key + "/size";
}

//! Returns the default dialog size.
QSize GetDefaultSize()
{
  return {mvvm::style::UnitSize(30), mvvm::style::UnitSize(30)};
}

}  // namespace

DetailedMessageBox::DetailedMessageBox(const MessageEvent& message, QWidget* parent_widget)
    : DetailedMessageBox(QString(), message, parent_widget)
{
}

DetailedMessageBox::DetailedMessageBox(const QString& setting_key, const MessageEvent& message,
                                       QWidget* parent_widget)
    : DetailedMessageBox(setting_key, nullptr, message, parent_widget)
{
}

DetailedMessageBox::DetailedMessageBox(const std::string& group, ISettingsManager& settings_manager,
                                       const MessageEvent& message, QWidget* parent_widget)
    : DetailedMessageBox(QString::fromStdString(group), &settings_manager, message, parent_widget)
{
}

DetailedMessageBox::DetailedMessageBox(const QString& setting_key,
                                       ISettingsManager* settings_manager,
                                       const MessageEvent& message, QWidget* parent_widget)
    : QDialog(parent_widget)
    , m_setting_group(setting_key)
    , m_settings_manager(settings_manager)
    , m_text_label(new QLabel(QString::fromStdString(message.text)))
    , m_detailed_text_edit(new QPlainTextEdit)
{
  setWindowTitle(QString::fromStdString(message.title));

  auto layout = new QVBoxLayout(this);
  layout->addLayout(CreateIconAndTextLayout().release());

  if (!message.informative.empty())
  {
    layout->addLayout(CreateInformativeLayout(message.informative).release());
  }

  layout->addWidget(m_detailed_text_edit);
  layout->addLayout(CreateOkOnlyButtonLayout(this).release());

  m_detailed_text_edit->setPlainText(QString::fromStdString(message.detailed));
  m_detailed_text_edit->setReadOnly(true);
  m_detailed_text_edit->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);

  ReadSettings();

  if (m_settings_manager == nullptr)
  {
    connect(this, &QDialog::accepted, this, &DetailedMessageBox::WriteSettings);
  }
}

QPlainTextEdit* DetailedMessageBox::GetTextEdit() const
{
  return m_detailed_text_edit;
}

std::unique_ptr<QLayout> DetailedMessageBox::CreateIconAndTextLayout()
{
  m_text_label->setWordWrap(true);

  auto result = std::make_unique<QHBoxLayout>();
  result->addLayout(CreateIconLayout().release());
  result->addWidget(m_text_label);
  const auto gap = mvvm::style::UnitSize(0.5);
  result->setContentsMargins(gap, gap, gap, gap);
  return result;
}

std::unique_ptr<QLayout> DetailedMessageBox::CreateInformativeLayout(const std::string& informative)
{
  m_informative_label = new QLabel(QString::fromStdString(informative));

  auto result = std::make_unique<QVBoxLayout>();
  m_informative_label->setWordWrap(true);
  result->addWidget(m_informative_label);
  const auto gap = mvvm::style::UnitSize(0.5);
  result->setContentsMargins(gap, gap, gap, gap);
  return result;
}

std::unique_ptr<QLayout> DetailedMessageBox::CreateIconLayout()
{
  auto result = std::make_unique<QVBoxLayout>();

  const QIcon icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);

  auto pixmap_label = new QLabel;
  pixmap_label->setPixmap(icon.pixmap(mvvm::style::UnitSize(5)));
  pixmap_label->setFixedWidth(mvvm::style::UnitSize(6));
  result->addWidget(pixmap_label);

  return result;
}

void DetailedMessageBox::done(int result)
{
  if (m_settings_manager != nullptr)
  {
    m_settings_manager->Set(m_setting_group.toStdString(), WindowInfo{saveGeometry()},
                            SettingsScope::kApplication);
  }
  QDialog::done(result);
}

void DetailedMessageBox::ReadSettings()
{
  if (m_setting_group.isEmpty())
  {
    return;
  }

  if (m_settings_manager != nullptr)
  {
    const auto info = m_settings_manager->Get<WindowInfo>(m_setting_group.toStdString(),
                                                          SettingsScope::kApplication);
    if (!restoreGeometry(info.geometry))
    {
      resize(GetDefaultSize());
    }
    return;
  }

  const QSettings settings;
  resize(settings.value(GetDialogSizeSettingName(m_setting_group), GetDefaultSize()).toSize());
}

void DetailedMessageBox::WriteSettings()
{
  if (m_setting_group.isEmpty())
  {
    return;
  }

  QSettings settings;
  settings.setValue(GetDialogSizeSettingName(m_setting_group), size());
}

}  // namespace sup::gui
