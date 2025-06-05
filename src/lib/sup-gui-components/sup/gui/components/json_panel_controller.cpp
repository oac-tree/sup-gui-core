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

namespace sup::gui
{

JsonPanelController::JsonPanelController(send_json_func_t send_json_func,
                                         send_message_func_t send_message_func)
    : m_send_json_func(std::move(send_json_func)), m_send_message_func(std::move(send_message_func))
{
  if (!m_send_json_func)
  {
    throw RuntimeException("Undefined callback to send JSON");
  }

  if (!m_send_message_func)
  {
    throw RuntimeException("Undefined callback to report errors");
  }
}

}  // namespace sup::gui
