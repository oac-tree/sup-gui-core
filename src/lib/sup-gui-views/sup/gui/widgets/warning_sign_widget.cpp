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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "warning_sign_widget.h"

#include "message_helper.h"

#include <sup/gui/style/style_helper.h>

#include <QIcon>
#include <QPainter>
#include <QRect>

namespace
{

/**
 * @brief Returns size of pixmap
 */
QSize GetPixmapSize()
{
  return sup::gui::utils::ToolBarIconSize();
}

/**
 * @brief Returns rectangle to draw a pixmap.
 */
QRect GetPixmapRect()
{
  auto size = GetPixmapSize();
  return {0, 0, size.width(), size.width()};
}

}  // namespace

namespace sup::gui
{

WarningSignWidget::WarningSignWidget(const MessageEvent &message, QWidget *parent_widget)
    : QWidget(parent_widget), m_message(message)
{
  setAttribute(Qt::WA_NoSystemBackground);
  setToolTip("Click to see details");

  auto icon = utils::FindIcon("bell-alert-outline");
  m_pixmap = icon.pixmap(GetPixmapSize());

  auto rect = GetPixmapRect();
  setGeometry(0, 0, rect.width(), rect.height());
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
  Q_UNUSED(event);

  m_is_busy = true;
  SendWarningMessage(m_message);
  m_is_busy = false;
}

}  // namespace sup::gui
