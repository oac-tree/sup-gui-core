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

#ifndef SUP_GUI_SETTINGS_SCOPED_CALLBACK_H_
#define SUP_GUI_SETTINGS_SCOPED_CALLBACK_H_

#include <functional>

namespace sup::gui
{

/**
 * @brief The ScopedCallback class calls a callback once, on destruction or on Disconnect().
 *
 * Move-only. Move assignment calls the callback of the replaced object.
 */
class ScopedCallback
{
public:
  ScopedCallback() = default;

  /**
   * @brief Main c-tor.
   *
   * @param callback The function to call.
   */
  explicit ScopedCallback(std::function<void()> callback);

  ~ScopedCallback();

  ScopedCallback(const ScopedCallback&) = delete;
  ScopedCallback& operator=(const ScopedCallback&) = delete;
  ScopedCallback(ScopedCallback&& other) noexcept;
  ScopedCallback& operator=(ScopedCallback&& other) & noexcept;

  /**
   * @brief Checks if the callback is not called yet.
   */
  bool IsConnected() const;

  /**
   * @brief Calls the callback. Does nothing if already called or empty.
   */
  void Disconnect();

private:
  std::function<void()> m_callback;
};

}  // namespace sup::gui

#endif  // SUP_GUI_SETTINGS_SCOPED_CALLBACK_H_
