/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "warning_sign_widget.h"

#include <sup/gui/style/style_helper.h>

#include <QIcon>
#include <QMessageBox>
#include <QPainter>
#include <QRect>

namespace
{

/**
 * @brief Returns rectangle to draw a pixmap.
 */
QRect GetPixmapRect()
{
  auto size = sup::gui::utils::ToolBarIconSize();
  return {0, 0, size.width(), size.width()};
}

}  // namespace

namespace sup::gui
{

WarningSignWidget::WarningSignWidget(const QString &message, QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_header("Houston, we have a problem.")
    , m_message(message)
    , m_pixmap(":/sup-gui/icons/bell-alert-outline.svg")
{
  setAttribute(Qt::WA_NoSystemBackground);
  setToolTip("Click to see details");

  auto rect = GetPixmapRect();
  setGeometry(0, 0, rect.width(), rect.height());
}

void WarningSignWidget::SetHeader(const QString &header)
{
  m_header = header;
}

bool WarningSignWidget::IsBusy() const
{
  return m_is_busy;
}

void WarningSignWidget::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.drawPixmap(GetPixmapRect(), m_pixmap);
}

void WarningSignWidget::mousePressEvent(QMouseEvent *event)
{
  m_is_busy = true;
  Q_UNUSED(event);
  QMessageBox box;
  box.setWindowTitle(m_header);
  box.setInformativeText(m_message);
  box.setStandardButtons(QMessageBox::Ok);
  box.setDefaultButton(QMessageBox::Ok);
  box.exec();
  m_is_busy = false;
}

}  // namespace sup::gui
