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
                                         send_text_func_t send_text_func,
                                         send_message_func_t send_message_func)
    : AbstractTextContentController(container, std::move(send_text_func),
                                    std::move(send_message_func))
    , m_container(container)
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
    // If model is inconsistent, CreateAnyValue method will fail.
    // This will be caught and reported by the base class.

    auto any_value = sup::gui::CreateAnyValue(*item);
    return sup::gui::AnyValueToJSONString(any_value, m_pretty_json);
  }

  return {};
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
