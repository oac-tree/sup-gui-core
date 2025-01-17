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

#ifndef SUP_GUI_WIDGETS_WARNING_SIGN_WIDGET_H_
#define SUP_GUI_WIDGETS_WARNING_SIGN_WIDGET_H_

#include <sup/gui/core/message_event.h>

#include <QPixmap>
#include <QWidget>

namespace sup::gui
{

/**
 * @brief The WarningSignWidget class represents the overlay warning sign indended to appear in the
 * corner of some widget.
 *
 * Provide dialog window on click.
 */
class WarningSignWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WarningSignWidget(const MessageEvent& message, QWidget* parent_widget = nullptr);

  bool IsBusy() const;

protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

private:
  QPixmap m_pixmap;
  bool m_is_busy{false};
  MessageEvent m_message;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_WARNING_SIGN_WIDGET_H_
