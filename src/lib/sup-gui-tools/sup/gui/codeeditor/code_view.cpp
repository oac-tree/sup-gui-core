/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "code_view.h"

#include <mvvm/utils/file_utils.h>

#include <sup/gui/codeeditor/code_editor.h>

#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QScrollBar>
#include <QSettings>
#include <QTextEdit>
#include <QTextStream>
#include <QToolBar>
#include <QVBoxLayout>
#include <fstream>

namespace
{

const QString kGroupName("CodeView");
const QString kWorkdirSettingName = kGroupName + "/" + "workdir";
}  // namespace

namespace sup::gui
{
CodeView::CodeView(QWidget *parent) : QWidget(parent), m_text_edit(new CodeEditor)
{
  setWindowTitle("XML");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_text_edit);

  SetupActions();
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
  const int old_scrollbar_value = m_text_edit->verticalScrollBar()->value();
  m_text_edit->clear();

  m_text_edit->SetText(content, "XML");
  m_text_edit->verticalScrollBar()->setValue(old_scrollbar_value);
}

void CodeView::ClearText()
{
  m_text_edit->clear();
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

void CodeView::SetupActions()
{
  auto export_to_file_action = new QAction("&Export XML", this);
  //  export_to_file_action->setIcon(styleutils::GetIcon("file-export-outline"));
  export_to_file_action->setToolTip("Export procedure to XML file");

  auto on_export_to_file = [this]()
  {
    auto file_name = QFileDialog::getSaveFileName(
        this, "Save File", m_current_workdir + "/untitled.xml", tr("Files (*.xml *.XML)"));

    if (!file_name.isEmpty())
    {
      auto parent_path = mvvm::utils::GetParentPath(file_name.toStdString());
      m_current_workdir = QString::fromStdString(parent_path);
      std::ofstream file_out(file_name.toStdString());
      file_out << m_text_edit->toPlainText().toStdString();
      file_out.close();
    }
  };

  connect(export_to_file_action, &QAction::triggered, this, on_export_to_file);
  addAction(export_to_file_action);
}

}  // namespace sup::gui
