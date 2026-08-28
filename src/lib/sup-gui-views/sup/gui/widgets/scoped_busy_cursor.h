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

#ifndef SUP_GUI_WIDGETS_SCOPED_BUSY_CURSOR_H_
#define SUP_GUI_WIDGETS_SCOPED_BUSY_CURSOR_H_

class QWidget;

namespace sup::gui
{

/**
 * @brief The ScopedBusyCursor class is a RAII wrapper to change the cursor appearance to "busy",
 * and back.
 *
 * The class implements special cursor handling for Wayland, which ignores cursor change in the
 * following situation:
 * - user opens a modal dialog, and then close dialog
 * - we show busy cursor
 *   - Wayland ignores cursor override while we are in transition from modal dialog
 * - heavy operation follows
 * - we show normal cursor
 *
 * The class implement waiting logic while focus event to the widget is still in flight.
 */
class ScopedBusyCursor
{
public:
  /**
   * @brief Main c-tor.
   *
   * @param widget The widget on behalf of which the long operation is performed. It is used to
   * find the window that should own the pointer focus. When nullptr, the currently active window
   * is used instead.
   */
  explicit ScopedBusyCursor(QWidget* widget = nullptr);
  ~ScopedBusyCursor();

  ScopedBusyCursor(const ScopedBusyCursor&) = delete;
  ScopedBusyCursor& operator=(const ScopedBusyCursor&) = delete;
  ScopedBusyCursor(ScopedBusyCursor&&) = delete;
  ScopedBusyCursor& operator=(ScopedBusyCursor&&) = delete;
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_SCOPED_BUSY_CURSOR_H_
