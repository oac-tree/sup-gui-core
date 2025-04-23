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

#include "code_view.h"

#include <sup/gui/views/codeeditor/code_editor.h>

#include <mvvm/utils/file_utils.h>

#include <QFile>
#include <QFileDialog>
#include <QScrollBar>
#include <QSettings>
#include <QTextStream>
#include <QToolBar>
#include <QVBoxLayout>

namespace
{

const QString kGroupName("CodeView");
const QString kWorkdirSettingName = kGroupName + "/" + "workdir";

QString SuggestFileName(sup::gui::CodeView::LanguageDefinition language)
{
  return language == sup::gui::CodeView::kXML ? "untitled.xml" : "untitled.json";
}

QString LanguageName(sup::gui::CodeView::LanguageDefinition language)
{
  return language == sup::gui::CodeView::kXML ? "XML" : "JSON";
}

}  // namespace

namespace sup::gui
{
CodeView::CodeView(LanguageDefinition language, QWidget *parent_widget)
    : QWidget(parent_widget), m_text_edit(new CodeEditor), m_language(language)
{
  setWindowTitle(LanguageName(language) + " View");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_text_edit);

  m_text_edit->SetDefinition(LanguageName(language));
  m_text_edit->setReadOnly(true);

  ReadSettings();
}

CodeView::~CodeView()
{
  WriteSettings();
}

void CodeView::SetFile(const QString &file_name)
{
  QFile file(file_name);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    return;
  }

  QTextStream in(&file);
  QString text;
  while (!in.atEnd())
  {
    text.append(in.readLine() + "\n");
  }

  SetContent(text);
}

void CodeView::SetContent(const QString &content)
{
  SaveScrollBarPosition();

  m_text_edit->clear();
  m_text_edit->SetText(content);

  RestoreScrollBarPosition();
}

void CodeView::ClearText()
{
  m_text_edit->clear();
}

void CodeView::OnExportToFileRequest()
{
  auto file_name = QFileDialog::getSaveFileName(
      this, "Save File", m_current_workdir + "/" + SuggestFileName(m_language),
      tr("XML files (*.xml *.XML);;JSON files (*.json)"));

  if (!file_name.isEmpty())
  {
    auto parent_path = mvvm::utils::GetParentPath(file_name.toStdString());
    m_current_workdir = QString::fromStdString(parent_path);
    QFile file(file_name);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream out(&file);
      out << m_text_edit->toPlainText();
    }
  }
}

void CodeView::ReadSettings()
{
  const QSettings settings;
  m_current_workdir = settings.value(kWorkdirSettingName, QDir::homePath()).toString();
}

void CodeView::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWorkdirSettingName, m_current_workdir);
}

void CodeView::SaveScrollBarPosition()
{
  // We save scroll bar position only if current position is not zero.
  // This is a simple way to ignore moments, when text editor was cleared because of
  // inconsistency in the model.

  if (const int pos = m_text_edit->verticalScrollBar()->value(); pos > 0)
  {
    m_cached_scrollbar_pos.vertical = pos;
  }

  if (const int pos = m_text_edit->horizontalScrollBar()->value(); pos > 0)
  {
    m_cached_scrollbar_pos.horizontal = pos;
  }
}

void CodeView::RestoreScrollBarPosition()
{
  m_text_edit->verticalScrollBar()->setValue(m_cached_scrollbar_pos.vertical);
  m_text_edit->horizontalScrollBar()->setValue(m_cached_scrollbar_pos.horizontal);
}

}  // namespace sup::gui
