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

std::string GetElementPrefix(const sup::gui::AnyValueItem& parent)
{
  const static std::map<std::string, std::string> type_to_prefix{
      {sup::gui::AnyValueStructItem::Type, sup::gui::kFieldNamePrefix},
      {sup::gui::AnyValueArrayItem::Type, sup::gui::kElementNamePrefix}};

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

  if (child.GetType() == AnyValueScalarItem::Type || child.GetType() == AnyValueEmptyItem::Type)
  {
    return kAnyValueDefaultDisplayName;
  }

  return {}; // no good suggestions
}

}  // namespace sup::gui
