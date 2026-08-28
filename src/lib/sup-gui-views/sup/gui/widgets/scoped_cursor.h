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

#ifndef SUP_GUI_WIDGETS_SCOPED_CURSOR_H_
#define SUP_GUI_WIDGETS_SCOPED_CURSOR_H_

#include <qnamespace.h>

class QWidget;

namespace sup::gui
{

/**
 * @brief The ScopedCursor class is a RAII wrapper to change the cursor appearance to the user
 * provided value, and then back.
 *
 * The class implements special cursor handling for Wayland, which ignores cursor change in the
 * following situation:
 * - user opens some modal dialog, and then close it
 * - at this moment we want to show busy cursor
 *   - Wayland ignores attempts to change the cursor while we are in transition from modal dialog
 * - heavy operation follows (but cursor is still normal)
 *
 * The class implement waiting logic while focus event to the widget is still in flight.
 */
class ScopedCursor
{
public:
  /**
   * @brief Main c-tor.
   *
   * Will change the cursor.
   *
   * @param widget The widget on behalf of which the long operation is performed.
   * @param cursor_shape The shape of the cursor to keep while object lives.
   */
  explicit ScopedCursor(QWidget* widget = nullptr,
                        Qt::CursorShape cursor_shape = Qt::CursorShape::WaitCursor);

  /**
   * d-tor, will restore cursor.
   */
  ~ScopedCursor();

  ScopedCursor(const ScopedCursor&) = delete;
  ScopedCursor& operator=(const ScopedCursor&) = delete;
  ScopedCursor(ScopedCursor&&) = delete;
  ScopedCursor& operator=(ScopedCursor&&) = delete;

private:
  Qt::CursorShape m_cursor_shape; //! cursor to show while alive
};

}  // namespace sup::gui

#endif  // SUP_GUI_WIDGETS_SCOPED_CURSOR_H_
