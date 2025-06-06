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

#include <sup/gui/components/abstract_text_content_controller.h>

namespace sup::gui
{

class AnyValueItem;

/**
 * @brief The JsonPanelController class generates JSON representation on AnyValueItem on every
 * change in the model.
 */
class JsonPanelController : public AbstractTextContentController
{
public:
  /**
   * @brief Main c-tor.
   *
   * @param container Container with AnyValueItem in it.
   * @param send_text_func A function to send generated JSON.
   * @param send_message_func A function to report errors.
   */
  JsonPanelController(mvvm::SessionItem* container, send_text_func_t send_text_func,
                      send_message_func_t send_message_func);

  ~JsonPanelController() override;
  JsonPanelController(const JsonPanelController&) = delete;
  JsonPanelController& operator=(const JsonPanelController&) = delete;
  JsonPanelController(JsonPanelController&&) = delete;
  JsonPanelController& operator=(JsonPanelController&&) = delete;

  void SetPrettyJson(bool value);

  bool IsPrettyJson() const;

private:
  std::string GenerateText() override;
  AnyValueItem* GetAnyValueItem();

  mvvm::SessionItem* m_container{nullptr};
  bool m_pretty_json{false};
};

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_JSON_PANEL_CONTROLLER_H_
