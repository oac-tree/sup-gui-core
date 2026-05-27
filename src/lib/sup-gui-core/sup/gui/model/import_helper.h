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

#ifndef SUP_GUI_MODEL_ANYVALUE_ITEM_UTILS_H_
#define SUP_GUI_MODEL_ANYVALUE_ITEM_UTILS_H_

#include <functional>
#include <memory>
#include <string>

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{

class AnyValueItem;
class MessageEvent;

/**
 * @brief Creates AnyValueItem from JSON content in given file.
 *
 * @param file_name The full name of the file.
 * @param report_error The callback to report error.
 */
std::unique_ptr<AnyValueItem> CreateAnyValueItemFromFile(
    const std::string& file_name, const std::function<void(const MessageEvent&)>& report_error);

/**
 * @brief Will import AnyValue from file and append it as ConfigurationItem to given container.
 *
 * The file should contain JSON with AnyValue content.
 */
void AppendConfigurationFromFile(const std::string& file_name,
                                 const std::function<void(const MessageEvent&)>& report_error,
                                 mvvm::SessionItem& container);

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_ANYVALUE_ITEM_UTILS_H_
