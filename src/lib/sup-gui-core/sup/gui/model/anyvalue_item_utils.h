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

//! Update the data stored in \it target from the data of \it source.
//! It is assumed that both items represent scalars, and the scalar types are the same.
void UpdateAnyValueItemScalarData(const AnyValueItem& source, AnyValueItem& target);

//! Update the data stored in leaves of \it target from the data stored in leaves of \it source.
//! The layout of two items should be the same.
void UpdateAnyValueItemData(const AnyValueItem& source, AnyValueItem& target);

//! Returns true if given \it scalar_type is suitable for the array.
//! If array is empty, will always return true. If array is not empty, \it scalar_type should
//! coincide with what is already in the array.
bool IsSuitableScalarType(const AnyValueArrayItem& array, const std::string& scalar_type);

//! Return a list of valid AnyValueItem type strings

std::vector<std::string> GetAnyValueItemTypes();

//! Create a TagInfo for a AnyValueItem (of any valid kind)

mvvm::TagInfo CreateAnyValueTag(std::string name, int min = 0, int max = -1);

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_ANYVALUE_ITEM_UTILS_H_
