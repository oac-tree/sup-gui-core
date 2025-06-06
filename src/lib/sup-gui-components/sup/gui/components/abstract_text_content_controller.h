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

#ifndef SUP_GUI_COMPONENTS_ABSTRACT_TEXT_CONTENT_CONTROLLER_H_
#define SUP_GUI_COMPONENTS_ABSTRACT_TEXT_CONTENT_CONTROLLER_H_

#include <sup/gui/core/message_event.h>

#include <mvvm/signals/event_types.h>

#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace mvvm
{
class SessionItem;
class ModelListener;
}  // namespace mvvm

namespace sup::gui
{

/**
 * @brief The AbstractTextContentController partially implements functionality for all JSON/XML
 * generators.
 *
 * It listens for updates in SessionModel and send to the user its representation (Sequencer's XML,
 * or AnyValue's JSON).
 */
class AbstractTextContentController
{
public:
  using send_text_func_t = std::function<void(const std::string&)>;
  using send_message_func_t = std::function<void(const sup::gui::MessageEvent& message)>;

  /**
   * @brief Main c-tor.
   *
   * @param container COntainer containing our object of interest.
   * @param send_text_func A function to send generated text.
   * @param send_message_func A function to report errors.
   */
  AbstractTextContentController(mvvm::SessionItem* container, send_text_func_t send_text_func,
                                send_message_func_t send_message_func);

  ~AbstractTextContentController();
  AbstractTextContentController(const AbstractTextContentController&) = delete;
  AbstractTextContentController& operator=(const AbstractTextContentController&) = delete;
  AbstractTextContentController(AbstractTextContentController&&) = delete;
  AbstractTextContentController& operator=(AbstractTextContentController&&) = delete;

private:
  void SetupListener();
  void UpdateText();
  virtual std::string GetTextImpl() = 0;
  virtual void OnDataChangedEvent(const mvvm::DataChangedEvent& event);
  virtual void OnAboutToRemoveItemEvent(const mvvm::AboutToRemoveItemEvent& event);

  /**
   * @brief Notifies the user that JSON generation went wrong.
   */
  void SendMessage(const std::string& what) const;

  /**
   * @brief Sends JSON to client.
   */
  void SendText(const std::string& str);

  mvvm::SessionItem* m_container{nullptr};
  send_text_func_t m_send_text_func;
  send_message_func_t m_send_message_func;
  std::unique_ptr<mvvm::ModelListener> m_listener;
  std::optional<std::string> m_last_send_text;
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_ABSTRACT_TEXT_CONTENT_CONTROLLER_H_
