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

#include "scoped_busy_cursor.h"

#include <QApplication>
#include <QElapsedTimer>
#include <QWidget>

namespace
{

/**
 * @brief The maximum time we are ready to spend waiting for the pointer focus.
 */
const int kPointerFocusTimeoutMsec = 150;

/**
 * @brief Waits until the mouse pointer has entered the window of the given widget.
 */
void WaitForPointerFocus(QWidget* widget)
{
  auto window = (widget == nullptr) ? QApplication::activeWindow() : widget->window();
  if (window == nullptr)
  {
    return;
  }

  QElapsedTimer timer;
  timer.start();
  while (!window->underMouse() && (timer.elapsed() < kPointerFocusTimeoutMsec))
  {
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 10);
  }
}

}  // namespace

namespace sup::gui
{

ScopedBusyCursor::ScopedBusyCursor(QWidget* widget)
{
  // We might be in unlucky moment of time, when enter event to the widget is still in flight.
  // This can happen if we had a modal dialog just closed. Waiting for widget become in focus.
  WaitForPointerFocus(widget);

  QApplication::setOverrideCursor(Qt::WaitCursor);
  QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

ScopedBusyCursor::~ScopedBusyCursor()
{
  QApplication::restoreOverrideCursor();
  QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

}  // namespace sup::gui
