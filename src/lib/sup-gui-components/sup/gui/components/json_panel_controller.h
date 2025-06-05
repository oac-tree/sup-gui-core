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
#include <memory>
#include <string>

namespace mvvm
{
class ModelListener;
}

namespace sup::gui
{

class AnyValueItem;

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
   * @param container COntainer with AnyValueItem in it.
   * @param send_json_func A function to send generated JSON.
   * @param send_message_func A function to report errors.
   */
  JsonPanelController(mvvm::SessionItem* container, send_json_func_t send_json_func,
                      send_message_func_t send_message_func);

  ~JsonPanelController();
  JsonPanelController(const JsonPanelController&) = delete;
  JsonPanelController& operator=(const JsonPanelController&) = delete;
  JsonPanelController(JsonPanelController&&) = delete;
  JsonPanelController& operator=(JsonPanelController&&) = delete;

  void SetPrettyJson(bool value);

private:
  void SetupListener();
  void UpdateJson();
  void OnDataChangedEvent(const mvvm::DataChangedEvent& event);
  void OnAboutToRemoveItemEvent(const mvvm::AboutToRemoveItemEvent& event);

  /**
   * @brief Notifies the user that JSON generation went wrong.
   */
  void SendMessage(const std::string& what) const;

  AnyValueItem* GetAnyValueItem();

  mvvm::SessionItem* m_container{nullptr};
  send_json_func_t m_send_json_func;
  send_message_func_t m_send_message_func;
  std::unique_ptr<mvvm::ModelListener> m_listener;
  bool m_pretty_json{false};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_JSON_PANEL_CONTROLLER_H_
