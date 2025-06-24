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

#include "anyvalue_editor_helper.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <mvvm/utils/container_utils.h>

#include <map>

namespace sup::gui
{

bool HasOneOfDefaultDisplayNames(const mvvm::SessionItem& child)
{
  static const std::vector<std::string> default_names = {
      constants::kAnyValueDefaultDisplayName, constants::kStructTypeName, constants::kArrayTypeName,
      constants::kScalarTypeName, constants::kEmptyTypeName};
  return mvvm::utils::Contains(default_names, child.GetDisplayName());
}

std::optional<std::string> SuggestDisplayName(const mvvm::SessionItem& parent,
                                              const mvvm::SessionItem& child)
{
  // For struct field we are suggesting names: field0, field1, etc, unless there is already
  // non-default field name, which came probably from another copy-and-paste operation.
  if (parent.GetType() == sup::gui::AnyValueStructItem::GetStaticType())
  {
    if (HasOneOfDefaultDisplayNames(child))
    {
      return sup::gui::constants::kFieldNamePrefix
             + std::to_string(parent.GetItemCount(constants::kAnyValueChildrenTag));
    }

    return {};  // item has good name already
  }

  // For array element we suggest names: element0, element1, etc.
  if (parent.GetType() == sup::gui::AnyValueArrayItem::GetStaticType())
  {
    return sup::gui::constants::kElementNamePrefix
           + std::to_string(parent.GetItemCount(constants::kAnyValueChildrenTag));
  }

  return constants::kAnyValueDefaultDisplayName;
}

std::optional<std::string> SuggestEditableTypeName(const mvvm::SessionItem& parent,
                                                   const mvvm::SessionItem& child)
{
  (void)parent;

  const static std::map<std::string, std::string> type_to_name{
      {AnyValueStructItem::GetStaticType(), constants::kStructTypeName},
      {AnyValueArrayItem::GetStaticType(), constants::kArrayTypeName}};

  // scalars has type name already
  auto iter = type_to_name.find(child.GetType());

  if (iter == type_to_name.end())
  {
    return {};
  }

  if(auto casted = dynamic_cast<const AnyValueItem*>(&child); casted)
  {
    if (casted->GetAnyTypeName().empty())
    {
      return iter->second;
    }
  }

  return {};

  // return iter == type_to_name.end() ? std::nullopt : std::optional<std::string>(iter->second);
}

void UpdateChildAppearance(const mvvm::SessionItem& parent, mvvm::SessionItem& child)
{
  if (auto anyvalue_item = dynamic_cast<AnyValueItem*>(&child); anyvalue_item)
  {
    if (auto name = SuggestDisplayName(parent, child); name.has_value())
    {
      (void)child.SetDisplayName(name.value());
    }

    if (auto name = SuggestEditableTypeName(parent, child); name.has_value())
    {
      anyvalue_item->SetAnyTypeName(name.value());
    }
  }
}

bool EnableInstantFieldNameEdit(const mvvm::SessionItem& child)
{
  if (auto parent = child.GetParent(); parent)
  {
    if (parent->GetType() == AnyValueStructItem::GetStaticType())
    {
      return true;
    }
  }

  return false;
}

void UpdateArrayElementNames(const mvvm::SessionItem& parent)
{
  if (auto array_item = dynamic_cast<const AnyValueArrayItem*>(&parent); array_item)
  {
    std::uint32_t index{0};
    for (auto child : array_item->GetChildren())
    {
      const auto name = sup::gui::constants::kElementNamePrefix + std::to_string(index++);
      child->SetDisplayName(name);
    }
  }
}

}  // namespace sup::gui
