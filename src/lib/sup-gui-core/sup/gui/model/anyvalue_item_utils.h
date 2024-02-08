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

#ifndef SUP_GUI_MODEL_ANYVALUE_ITEM_UTILS_H_
#define SUP_GUI_MODEL_ANYVALUE_ITEM_UTILS_H_

//! Collection of sup::gui::AnyValueItem related utility functions.

#include <mvvm/model/taginfo.h>

#include <string>
#include <vector>

namespace sup::gui
{

class AnyValueItem;
class AnyValueArrayItem;

/**
 * @brief Updates the data stored in leaves of a target from the data stored in leaves of a source.
 *
 * @details It is assumed that both items represent scalars, and the scalar types are the same.
 */
void UpdateAnyValueItemScalarData(const AnyValueItem& source, AnyValueItem& target);

/**
 * @brief Updates the data stored in leaves of a target from the data stored in leaves of a source.
 */
void UpdateAnyValueItemData(const AnyValueItem& source, AnyValueItem& target);

/**
 * @brief Returns true if the given scalar type is suitable for the array.
 *
 * @param array The array to explore.
 * @param scalar_type Possible type of the scalar to add to the array.
 * @return True if the scalar type is compatible with the array content.
 *
 * @details If the array is empty, will always return true. If the array is not empty, scalar_type
 * should coincide with what is already in the array.
 */
bool IsSuitableScalarType(const AnyValueArrayItem& array, const std::string& scalar_type);

/**
 * @brief Returns a list of all existing AnyValueItem type names.
 */
std::vector<std::string> GetAnyValueItemTypes();

/**
 * @brief Creates a TagInfo for a AnyValueItem (of any valid kind).
 *
 * @param name The tag name.
 *
 * @param min Minimum allowed number of items.
 * @param max Maximum allowed number of items.
 *
 * @return TagInfo object containing necessary information.
 */
mvvm::TagInfo CreateAnyValueTag(std::string name, int min = 0, int max = -1);

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_ANYVALUE_ITEM_UTILS_H_
