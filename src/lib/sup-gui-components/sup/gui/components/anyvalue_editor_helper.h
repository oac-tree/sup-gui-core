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

#ifndef SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_HELPER_H_
#define SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_HELPER_H_

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

/**
 * @brief Checks if given item has one of default display names.
 */
bool HasOneOfDefaultDisplayNames(const mvvm::SessionItem& child);

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
std::optional<std::string> SuggestDisplayName(const mvvm::SessionItem& parent,
                                              const mvvm::SessionItem& child);

/**
 * @brief Suggests type name for a child.
 *
 * @param parent A parent where a child will be inserted.
 * @param child Future child of a parent.
 * @return Optional string.
 */
std::optional<std::string> SuggestEditableTypeName(const mvvm::SessionItem& parent,
                                                   const mvvm::SessionItem& child);

/**
 * @brief Updates display name and type name for a given child in the context of a given parent.
 *
 * @param parent A parent where a child will be inserted.
 * @param child Future child of a parent.
 */
void UpdateChildAppearance(const mvvm::SessionItem& parent, mvvm::SessionItem& child);

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_ANYVALUE_EDITOR_HELPER_H_
