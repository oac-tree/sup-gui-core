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

#include "placeholder_widget.h"

#include <mvvm/widgets/widget_utils.h>

#include <QGridLayout>
#include <QLabel>

namespace sup::gui
{

namespace
{

constexpr auto kHeader = R"(
<html><body style="color:#909090; font-size:14px">
<div align='center'>
<div style="font-size:18px">%1</div>
<table><tr><td>
)";

constexpr auto kFooter = R"(
</td></tr></table>
</div>
</body></html>
)";

constexpr auto kDiv1 = R"(
<div style="margin-top: 12px">%1</div>
)";

QString CreatePlaceholderText(const QString& header_text, const QString& secondary_text = {})
{
  const QString header = QString(kHeader).arg(header_text);
  const QString middle = QString(kDiv1).arg(secondary_text);
  const QString footer(kFooter);

  return header + middle + footer;
}

}  // namespace

PlaceholderWidget::PlaceholderWidget(const QString& header_text, QWidget* parent_widget)
    : QWidget(parent_widget), m_label(new QLabel(CreatePlaceholderText(header_text)))
{
  // thanks to the grid layout our single label will be in the middle
  auto layout = new QGridLayout(this);
  layout->addWidget(m_label);

  connect(m_label, &QLabel::linkActivated, this, &PlaceholderWidget::linkActivated);
}

}  // namespace sup::gui
