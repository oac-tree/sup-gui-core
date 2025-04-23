/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Rafael Siva (EXT)
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

#include "about_application_dialog.h"

#include <mvvm/widgets/widget_utils.h>

#include <QDate>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

namespace sup::gui
{

AboutApplicationDialog::AboutApplicationDialog(QWidget* parent, const QString& app_name,
                                               const QString& description, const QString& version)
    : QDialog(parent)
{
  setWindowTitle("About " + app_name);

  auto details_layout = new QHBoxLayout;
  details_layout->addLayout(CreateLogoLayout().release());
  details_layout->addLayout(CreateTextLayout(app_name, description, version).release());

  auto main_layout = new QVBoxLayout(this);
  main_layout->addLayout(details_layout);
  main_layout->addLayout(CreateButtonLayout().release());
}

void AboutApplicationDialog::ShowDialog(QWidget* parent, const QString& app_name,
                                        const QString& description, const QString& version)
{
  AboutApplicationDialog dialog(parent, app_name, description, version);
  dialog.exec();
}

QString AboutApplicationDialog::GetCopyRight()
{
  QDate date = QDate::currentDate();
  return QString("Copyright: 2010-%1 ITER Organization ").arg(date.toString("yyyy"));
}

std::unique_ptr<QLabel> AboutApplicationDialog::CreateLinkLabel(const QString& link,
                                                                const QString& name)
{
  auto link_label = std::make_unique<QLabel>();
  link_label->setTextFormat(Qt::RichText);
  link_label->setTextInteractionFlags(Qt::TextBrowserInteraction);
  link_label->setOpenExternalLinks(true);
  link_label->setText(QString("<a href=\"%1\">%2</a>").arg(link, name));
  return link_label;
}

std::unique_ptr<QLayout> AboutApplicationDialog::CreateLogoLayout()
{
  auto layout = std::make_unique<QVBoxLayout>();
  auto label = new QLabel;

  label->setPixmap(
      QPixmap(":/sup-gui-core/icons/iter_logo.png")
          .scaled(mvvm::utils::UnitSize(10), mvvm::utils::UnitSize(10), Qt::KeepAspectRatio));

  layout->addWidget(label);
  layout->addStretch(1);

  const auto gap = mvvm::utils::UnitSize(1.0);
  layout->setContentsMargins(0, gap, gap, gap);

  return layout;
}

std::unique_ptr<QLayout> AboutApplicationDialog::CreateTextLayout(const QString& app_name,
                                                                  const QString& description,
                                                                  const QString& version)
{
  auto layout = std::make_unique<QVBoxLayout>();

  // Title
  auto about_title_label = new QLabel(app_name + " version " + version);
  mvvm::utils::ScaleLabelFont(about_title_label, 1.2, true);
  about_title_label->setContentsMargins(0, 0, 0, mvvm::utils::UnitSize());

  // Copyright
  auto copyright_label = new QLabel(GetCopyRight());

  // Description
  auto description_label = new QLabel(description);
  description_label->setWordWrap(true);

  layout->addWidget(about_title_label);
  layout->addWidget(description_label);
  layout->addStretch(1);

  // Copyright
  layout->addWidget(copyright_label);
  layout->addWidget(
      CreateLinkLabel("https://git.iter.org/projects/COA/repos", "CODAC Operational Applications")
          .release());
  layout->addStretch(1);

  auto gap = mvvm::utils::UnitSize(1.0);
  layout->setContentsMargins(0, gap, gap, gap);

  return layout;
}

std::unique_ptr<QLayout> AboutApplicationDialog::CreateButtonLayout()
{
  auto layout = std::make_unique<QVBoxLayout>();

  auto closeButton = new QPushButton("Close");
  connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);

  layout->addStretch(1);
  layout->addWidget(closeButton);

  return layout;
}

}  // namespace sup::gui
