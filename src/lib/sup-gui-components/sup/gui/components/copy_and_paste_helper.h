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

#ifndef SUP_GUI_COMPONENTS_COPY_AND_PASTE_HELPER_H_
#define SUP_GUI_COMPONENTS_COPY_AND_PASTE_HELPER_H_

//! @file
//! Collection of helper methods for copy-and-paste operations.

#include <vector>

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{

/**
 * @brief Returns the list of items which are located on the top level.
 *
 * The method is used in the copy-and-paste scenario when a large tree is partly copied while only
 * some children and parents are selected.
 *
 * Example:
 *
 * parent0
 *   child0   <-selected
 * parent1    <-selected
 *   child1   <-selected
 *   child2
 * parent2    <-selected
 *   child3   <-selected
 * parent3    <-selected
 *
 * For the list of items selection [child0, parent1, child1, parent2, child3, parent3] the result
 * will be [parent1, parent2, parent3].
 */
std::vector<mvvm::SessionItem*> GetTopLevelSelection(const std::vector<mvvm::SessionItem*>& items);

/**
 * @brief Removes all children from the given list.
 *
 * Assuming that the list contains a mixture of parents and their children, we will remove all items
 * that have their parents/grandparents located in the same list.  The method is used in the
 * cut/remove scenario when the user removes an arbitrary selection from a tree. We want to delete
 * only top-level items since all children will be removed anyway.
 *
 * @param items Mixture of parents and their children.
 * @return List of parent items.
 */
std::vector<mvvm::SessionItem*> FilterOutChildren(const std::vector<mvvm::SessionItem*>& items);

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_COPY_AND_PASTE_HELPER_H_
