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

#ifndef SUP_GUI_COMPONENTS_JSON_PANEL_CONTROLLER_H_
#define SUP_GUI_COMPONENTS_JSON_PANEL_CONTROLLER_H_

#include <sup/gui/core/message_event.h>

#include <mvvm/signals/event_types.h>

#include <functional>
#include <string>

namespace sup::gui
{

/**
 * @brief The JsonPanelController class generates JSON representation on AnyValueItem on every
 * change in the model.
 */
class JsonPanelController
{
public:
  using send_json_func_t = std::function<void(const std::string&)>;
  using send_message_func_t = std::function<void(const sup::gui::MessageEvent& message)>;

  /**
   * @brief Main c-tor.
   *
   * @param send_json_func A function to send generated JSON.
   * @param send_message_func A function to report errors.
   */
  JsonPanelController(send_json_func_t send_json_func, send_message_func_t send_message_func);

private:
  send_json_func_t m_send_json_func;
  send_message_func_t m_send_message_func;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_JSON_PANEL_CONTROLLER_H_
