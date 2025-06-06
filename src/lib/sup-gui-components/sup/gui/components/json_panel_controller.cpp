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

#include "json_panel_controller.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/model/session_item.h>
#include <mvvm/signals/model_listener.h>

#include <sup/dto/anyvalue.h>

namespace sup::gui
{

JsonPanelController::JsonPanelController(mvvm::SessionItem *container,
                                         send_json_func_t send_json_func,
                                         send_message_func_t send_message_func)
    : m_container(container)
    , m_send_json_func(std::move(send_json_func))
    , m_send_message_func(std::move(send_message_func))
{
  if (!m_container)
  {
    throw RuntimeException("Unitialised container");
  }

  if (!m_send_json_func)
  {
    throw RuntimeException("Undefined callback to send JSON");
  }

  if (!m_send_message_func)
  {
    throw RuntimeException("Undefined callback to report errors");
  }

  SetupListener();
  UpdateJson();
}

JsonPanelController::~JsonPanelController() = default;

void JsonPanelController::SetPrettyJson(bool value)
{
  m_pretty_json = value;
  UpdateJson();
}

void JsonPanelController::SetupListener()
{
  m_listener = std::make_unique<mvvm::ModelListener>(m_container->GetModel());
  m_listener->Connect<mvvm::ItemRemovedEvent>([this](const auto &) { UpdateJson(); });
  m_listener->Connect<mvvm::ItemInsertedEvent>([this](const auto &) { UpdateJson(); });
  m_listener->Connect<mvvm::DataChangedEvent>(this, &JsonPanelController::OnDataChangedEvent);
  m_listener->Connect<mvvm::AboutToRemoveItemEvent>(this,
                                                    &JsonPanelController::OnAboutToRemoveItemEvent);
}

void JsonPanelController::UpdateJson()
{
  if (auto item = GetAnyValueItem(); item)
  {
    try
    {
      // Current simplified approach calls the method `UpdateJson` on every
      // model change. If model is inconsistent, CreateAnyValue method will fail.

      auto any_value = sup::gui::CreateAnyValue(*item);
      auto str = sup::gui::AnyValueToJSONString(any_value, m_pretty_json);
      SendJson(str);
    }
    catch (const std::exception &ex)
    {
      SendJson(std::string());
      SendMessage(ex.what());
    }
  }
  else
  {
    SendJson(std::string());
  }
}

void JsonPanelController::OnDataChangedEvent(const mvvm::DataChangedEvent &event)
{
  (void)event;
  if (event.data_role == mvvm::DataRole::kData)
  {
    UpdateJson();
  }
}

void JsonPanelController::OnAboutToRemoveItemEvent(const mvvm::AboutToRemoveItemEvent &event)
{
  if (event.item->GetItem(event.tag_index) == m_container)
  {
    // container was deleted, stopping listening
    SendJson(std::string());
    m_container = nullptr;
    m_listener.reset();
  }
}

void JsonPanelController::SendMessage(const std::string &what) const
{
  const std::string title("JSON generation failed");
  const std::string text("The AnyValue being edited is in inconsistent state");
  const MessageEvent message{title, text, what, ""};
  m_send_message_func(message);
}

void JsonPanelController::SendJson(const std::string &str)
{
  // do not notify if we already send exactly same last time
  if (m_last_send_text.has_value() && m_last_send_text == str)
  {
    return;
  }

  m_last_send_text = str;
  m_send_json_func(str);
}

AnyValueItem *JsonPanelController::GetAnyValueItem()
{
  if (!m_container || m_container->GetTotalItemCount() == 0)
  {
    return nullptr;
  }

  return m_container->GetItem<AnyValueItem>(mvvm::TagIndex::Default(0));
}

}  // namespace sup::gui
