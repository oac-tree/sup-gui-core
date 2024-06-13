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

#include "anyvalue_editor_helper.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <map>

namespace
{

std::string GetElementPrefix(const mvvm::SessionItem& parent)
{
  const static std::map<std::string, std::string> type_to_prefix{
      {sup::gui::AnyValueStructItem::Type, sup::gui::constants::kFieldNamePrefix},
      {sup::gui::AnyValueArrayItem::Type, sup::gui::constants::kElementNamePrefix}};

  auto iter = type_to_prefix.find(parent.GetType());
  return iter == type_to_prefix.end() ? std::string() : iter->second;
}

}  // namespace

namespace sup::gui
{

std::optional<std::string> SuggestDisplayName(const mvvm::SessionItem& parent, AnyValueItem& child)
{
  if (auto anyvalue_item = dynamic_cast<const AnyValueItem*>(&parent); anyvalue_item)
  {
    // if parent is an AnyValueItem, suggest the name which is based on number of previous children
    return GetElementPrefix(*anyvalue_item) + std::to_string(anyvalue_item->GetChildrenCount());
  }

  return constants::kAnyValueDefaultDisplayName;
}

std::optional<std::string> SuggestEditableTypeName(const mvvm::SessionItem& parent,
                                                   AnyValueItem& child)
{
  if (child.IsStruct())
  {
    return constants::kStructTypeName;
  }

  if (child.IsArray())
  {
    return constants::kArrayTypeName;
  }

  return {};  // scalars has type name already
}

void UpdateChildAppearance(const mvvm::SessionItem& parent, AnyValueItem& child)
{
  if (auto name = SuggestDisplayName(parent, child); name.has_value())
  {
    child.SetDisplayName(name.value());
  }

  if (auto name = SuggestEditableTypeName(parent, child); name.has_value())
  {
    child.SetAnyTypeName(name.value());
  }
}

}  // namespace sup::gui
