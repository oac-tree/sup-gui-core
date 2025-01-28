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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalue_editor_helper.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <map>

namespace
{

std::string GetElementPrefix(const mvvm::SessionItem& parent)
{
  const static std::map<std::string, std::string> type_to_prefix{
      {sup::gui::AnyValueStructItem::GetStaticType(), sup::gui::constants::kFieldNamePrefix},
      {sup::gui::AnyValueArrayItem::GetStaticType(), sup::gui::constants::kElementNamePrefix}};

  auto iter = type_to_prefix.find(parent.GetType());
  return iter == type_to_prefix.end() ? std::string() : iter->second;
}

}  // namespace

namespace sup::gui
{

std::optional<std::string> SuggestDisplayName(const mvvm::SessionItem& parent,
                                              const mvvm::SessionItem& child)
{
  (void)child;

  const auto prefix = GetElementPrefix(parent);  // "field" for struct, and "element" for arrays

  // if prefix is valid, suggest the name which is based on number of previous children
  return prefix.empty()
             ? constants::kAnyValueDefaultDisplayName
             : prefix + std::to_string(parent.GetItemCount(constants::kAnyValueChildrenTag));
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
  return iter == type_to_name.end() ? std::nullopt : std::optional<std::string>(iter->second);
}

void UpdateChildAppearance(const mvvm::SessionItem& parent, mvvm::SessionItem& child)
{
  if (auto anyvalue_item = dynamic_cast<AnyValueItem*>(&child); anyvalue_item)
  {
    if (auto name = SuggestDisplayName(parent, child); name.has_value())
    {
      child.SetDisplayName(name.value());
    }

    if (auto name = SuggestEditableTypeName(parent, child); name.has_value())
    {
      anyvalue_item->SetAnyTypeName(name.value());
    }
  }
}

}  // namespace sup::gui
