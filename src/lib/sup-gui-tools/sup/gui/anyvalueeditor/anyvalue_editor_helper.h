/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_HELPER_H_
#define SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_HELPER_H_

//! @file
//! Helper functions for AnyValueEditor.

#include <optional>
#include <string>

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueItem;

const std::string kFieldNamePrefix = "field";
const std::string kElementNamePrefix = "element";
const std::string kAnyValueDefaultDisplayName = "value";

/**
 * @brief Suggests appropriate display name for a child.
 *
 * Method is used in the context of AnyValue editor action handler when an item is inserted in the
 * tree.
 *
 * @param parent A parent where a child will be inserted.
 * @param child Future child of a parent.
 * @return Optional string.
 */
std::optional<std::string> SuggestDisplayName(const mvvm::SessionItem& parent, AnyValueItem& child);

/**
 * @brief Suggests type name for a child.
 *
 * @param parent A parent where a child will be inserted.
 * @param child Future child of a parent.
 * @return Optional string.
 */
std::optional<std::string> SuggestEditableTypeName(const mvvm::SessionItem& parent, AnyValueItem& child);

}  // namespace sup::gui

#endif  // SUP_GUI_ANYVALUEEDITOR_ANYVALUE_EDITOR_HELPER_H_
