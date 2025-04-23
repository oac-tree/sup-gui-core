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

#ifndef SUP_GUI_COMPONENTS_ANYVALUE_ITEM_COPY_HELPER_H_
#define SUP_GUI_COMPONENTS_ANYVALUE_ITEM_COPY_HELPER_H_

//! @file
//! Collection of helper methods to copy arbitrary selection of AnyValueItems.

#include <vector>
#include <memory>

class QMimeData;

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{

class AnyValueItem;

/**
 * @brief Creates copy object for AnyValueItem selection tree.
 *
 * Selection tree is a list of items obtained when the user manually picks parent and child
 * AnyValueItem from a large tree.
 *
 * Example:
 * struct  <- selected
 *   field0
 *   field1 <- selected
 *
 * Selection list [struct, field1] will generate a copy object for the level "struct"
 * containing a single "field1". "fild0" child will be ignored.
 */
std::unique_ptr<QMimeData> CreateAnyValueItemSelectionCopyMimeData(
    const std::vector<AnyValueItem*>& selection);


/**
 * @brief Returns vector of AnyValueItems from given mime data.
 */
std::vector<std::unique_ptr<mvvm::SessionItem>> CreateAnyValueItems(const QMimeData* mime_data);

}  // namespace sup::gui

#endif  // SUP_GUI_COMPONENTS_COPY_AND_PASTE_HELPER_H_
