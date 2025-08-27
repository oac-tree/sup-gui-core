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

#include "detailed_message_box.h"

#include "dialog_helper.h"

#include <mvvm/widgets/widget_utils.h>

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QTextEdit>
#include <QVBoxLayout>

namespace sup::gui
{

DetailedMessageBox::DetailedMessageBox(const MessageEvent &message, QWidget *parent_widget)
    : QDialog(parent_widget)
    , m_text_label(new QLabel(QString::fromStdString(message.text)))
    , m_detailed_text_edit(new QTextEdit)
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

  m_detailed_text_edit->setText(QString::fromStdString(message.detailed));
  m_detailed_text_edit->setReadOnly(true);
}

QTextEdit *DetailedMessageBox::GetTextEdit() const
{
  return m_detailed_text_edit;
}

std::unique_ptr<QLayout> DetailedMessageBox::CreateIconAndTextLayout()
{
  m_text_label->setWordWrap(true);

  auto result = std::make_unique<QHBoxLayout>();
  result->addLayout(CreateIconLayout().release());
  result->addWidget(m_text_label);
  const auto gap = mvvm::utils::UnitSize(0.5);
  result->setContentsMargins(gap, gap, gap, gap);
  return result;
}

std::unique_ptr<QLayout> DetailedMessageBox::CreateInformativeLayout(const std::string &informative)
{
  m_informative_label = new QLabel(QString::fromStdString(informative));

  auto result = std::make_unique<QVBoxLayout>();
  m_informative_label->setWordWrap(true);
  result->addWidget(m_informative_label);
  const auto gap = mvvm::utils::UnitSize(0.5);
  result->setContentsMargins(gap, gap, gap, gap);
  return result;
}

std::unique_ptr<QLayout> DetailedMessageBox::CreateIconLayout()
{
  auto result = std::make_unique<QVBoxLayout>();

  QIcon icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);

  auto pixmap_label = new QLabel;
  pixmap_label->setPixmap(icon.pixmap(mvvm::utils::UnitSize(5)));
  pixmap_label->setFixedWidth(mvvm::utils::UnitSize(5) * 1.2);
  result->addWidget(pixmap_label);

  return result;
}

}  // namespace sup::gui
