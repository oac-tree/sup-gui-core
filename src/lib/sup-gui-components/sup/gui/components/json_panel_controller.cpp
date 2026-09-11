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

#include "json_panel_controller.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/signals/model_listener.h>

#include <sup/dto/anyvalue.h>

namespace sup::gui
{

namespace
{

/**
 * @brief Creates a message reporting that AnyValue is too large to be shown as JSON.
 */
MessageEvent CreateTooLargeMessage(std::size_t max_item_count)
{
  const std::string title("Content is too large to display");
  const std::string text("AnyValue contains more than " + std::to_string(max_item_count)
                         + " items");
  const std::string informative("Use Export action to save JSON representation into a file.");
  return MessageEvent{title, text, informative, ""};
}

}  // namespace

JsonPanelController::JsonPanelController(mvvm::SessionItem* container,
                                         send_text_func_t send_text_func,
                                         send_message_func_t send_message_func,
                                         std::size_t max_item_count)
    : AbstractTextContentController(container, std::move(send_text_func),
                                    std::move(send_message_func))
    , m_max_item_count(max_item_count)
{
  UpdateText();
}

JsonPanelController::~JsonPanelController() = default;

void JsonPanelController::SetPrettyJson(bool value)
{
  m_pretty_json = value;
  UpdateText();
}

bool JsonPanelController::IsPrettyJson() const
{
  return m_pretty_json;
}

std::string JsonPanelController::GenerateText()
{
  if (auto item = GetAnyValueItem(); item)
  {
    if (mvvm::utils::HasMoreItemsThan<AnyValueItem>(*item, m_max_item_count))
    {
      // do not generate JSON for large AnyValue, report via error message
      SendMessage(CreateTooLargeMessage(m_max_item_count));
      return {};
    }

    // If model is inconsistent, CreateAnyValue method will fail.
    // This will be caught and reported by the base class.

    auto any_value = CreateAnyValue(*item);
    return AnyValueToJSONString(any_value, m_pretty_json);
  }

  return {};
}

AnyValueItem* JsonPanelController::GetAnyValueItem()
{
  if ((GetContainer() == nullptr) || (GetContainer()->GetTotalItemCount() == 0))
  {
    return nullptr;
  }

  return GetContainer()->FindItem<AnyValueItem>(mvvm::TagIndex::Default(0));
}

}  // namespace sup::gui
