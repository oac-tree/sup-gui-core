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

#include "abstract_text_content_controller.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <mvvm/model/session_item.h>
#include <mvvm/signals/model_listener.h>

namespace sup::gui
{

AbstractTextContentController::AbstractTextContentController(mvvm::SessionItem *container,
                                                             send_text_func_t send_text_func,
                                                             send_message_func_t send_message_func)
    : m_container(container)
    , m_send_text_func(std::move(send_text_func))
    , m_send_message_func(std::move(send_message_func))
{
  if (!m_container)
  {
    throw RuntimeException("Unitialised container");
  }

  if (!m_send_text_func)
  {
    throw RuntimeException("Undefined callback to send JSON");
  }

  if (!m_send_message_func)
  {
    throw RuntimeException("Undefined callback to report errors");
  }

  SetupListener();
  UpdateText();
}

AbstractTextContentController::~AbstractTextContentController() = default;

void AbstractTextContentController::SetupListener()
{
  m_listener = std::make_unique<mvvm::ModelListener>(m_container->GetModel());
  m_listener->Connect<mvvm::ItemRemovedEvent>([this](const auto &) { UpdateText(); });
  m_listener->Connect<mvvm::ItemInsertedEvent>([this](const auto &) { UpdateText(); });
  m_listener->Connect<mvvm::DataChangedEvent>(this,
                                              &AbstractTextContentController::OnDataChangedEvent);
  m_listener->Connect<mvvm::AboutToRemoveItemEvent>(
      this, &AbstractTextContentController::OnAboutToRemoveItemEvent);
}

void AbstractTextContentController::UpdateText()
{
  try
  {
    auto str = GetTextImpl();
    SendText(str);
  }
  catch (const std::exception &ex)
  {
    SendText(std::string());
    SendMessage(ex.what());
  }
}

void AbstractTextContentController::OnDataChangedEvent(const mvvm::DataChangedEvent &event)
{
  (void)event;
  if (event.data_role == mvvm::DataRole::kData)
  {
    UpdateText();
  }
}

void AbstractTextContentController::OnAboutToRemoveItemEvent(
    const mvvm::AboutToRemoveItemEvent &event)
{
  if (event.item->GetItem(event.tag_index) == m_container)
  {
    // container was deleted, stopping listening
    SendText(std::string());
    m_container = nullptr;
    m_listener.reset();
  }
}

void AbstractTextContentController::SendMessage(const std::string &what) const
{
  const std::string title("JSON generation failed");
  const std::string text("The AnyValue being edited is in inconsistent state");
  const MessageEvent message{title, text, what, ""};
  m_send_message_func(message);
}

void AbstractTextContentController::SendText(const std::string &str)
{
  // do not notify if we already send exactly same last time
  if (m_last_send_text.has_value() && m_last_send_text == str)
  {
    return;
  }

  m_last_send_text = str;
  m_send_text_func(str);
}

}  // namespace sup::gui
