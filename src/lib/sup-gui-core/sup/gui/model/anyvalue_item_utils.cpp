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

#include "anyvalue_item_utils.h"

#include "anyvalue_conversion_utils.h"
#include "anyvalue_item.h"
#include "anyvalue_item_constants.h"

#include <sup/gui/core/exceptions.h>

#include <mvvm/model/function_types.h>
#include <mvvm/model/item_catalogue.h>

#include <algorithm>
#include <map>
#include <stack>

namespace
{

/**
 * @brief Creates factory to build AnyValueItem from corresponding AnyValueItem::Type.
 */
std::unique_ptr<mvvm::ItemCatalogue<sup::gui::AnyValueItem>> CreateItemCatalogue()
{
  auto result = std::make_unique<mvvm::ItemCatalogue<sup::gui::AnyValueItem>>();

  result->RegisterItem<sup::gui::AnyValueEmptyItem>();
  result->RegisterItem<sup::gui::AnyValueScalarItem>();
  result->RegisterItem<sup::gui::AnyValueArrayItem>();
  result->RegisterItem<sup::gui::AnyValueStructItem>();

  return result;
}

}  // namespace

namespace sup::gui
{

std::string GetAnyValueItemTypeFromTypeName(const std::string &type_name)
{
  static const std::map<std::string, std::string> kTypeNameToAnyValueItemType = {
      {sup::gui::constants::kEmptyTypeName, sup::gui::AnyValueEmptyItem::Type},
      {sup::gui::constants::kScalarTypeName, sup::gui::AnyValueScalarItem::Type},
      {sup::gui::constants::kArrayTypeName, sup::gui::AnyValueArrayItem::Type},
      {sup::gui::constants::kStructTypeName, sup::gui::AnyValueStructItem::Type},
  };

  if (IsScalarTypeName(type_name))
  {
    return sup::gui::AnyValueScalarItem::Type;
  }

  auto iter = kTypeNameToAnyValueItemType.find(type_name);
  return iter == kTypeNameToAnyValueItemType.end() ? std::string() : iter->second;
}

std::unique_ptr<AnyValueItem> CreateAnyValueItemFromTypeName(const std::string &type_name)
{
  static const auto item_catalogue = CreateItemCatalogue();

  if (IsScalarTypeName(type_name))
  {
    auto result = item_catalogue->Create(AnyValueScalarItem::Type);
    result->SetAnyTypeName(type_name);
    return result;
  }

  return item_catalogue->Create(GetAnyValueItemTypeFromTypeName(type_name));
}

void UpdateAnyValueItemScalarData(const AnyValueItem &source, AnyValueItem &target)
{
  if (!source.IsScalar() || !target.IsScalar())
  {
    throw std::logic_error("Item(s) are not scalars");
  }

  if (source.GetAnyTypeName() != target.GetAnyTypeName())
  {
    throw std::logic_error("Item types do not match");
  }

  target.SetData(source.Data());
}

void UpdateAnyValueItemData(const AnyValueItem &source, AnyValueItem &target)
{
  struct Node
  {
    const AnyValueItem *source{nullptr};
    AnyValueItem *target{nullptr};
  };

  std::stack<Node> nodes;
  nodes.push({&source, &target});

  while (!nodes.empty())
  {
    auto &node = nodes.top();

    if (node.source->IsScalar())
    {
      UpdateAnyValueItemScalarData(*node.source, *node.target);
      nodes.pop();
    }
    else
    {
      auto source_children = node.source->GetChildren();
      auto target_children = node.target->GetChildren();
      nodes.pop();

      if (source_children.size() != target_children.size())
      {
        throw RuntimeException(
            "While updating target AnyValue from source the different layout "
            "of target and source has been detected. The number of children "
            "does not match");
      }

      // preparing vector containing pairs of children
      std::vector<Node> new_children;
      auto on_transform = [&nodes](const AnyValueItem *source, AnyValueItem *target)
      { return Node{source, target}; };
      std::transform(source_children.begin(), source_children.end(), target_children.begin(),
                     std::back_inserter(new_children), on_transform);

      // adding to the stack in reverse order
      for (auto it = new_children.rbegin(); it != new_children.rend(); ++it)
      {
        nodes.push(*it);
      }
    }
  }
}

bool IsSuitableScalarType(const AnyValueArrayItem &array, const std::string &scalar_type)
{
  if (array.GetChildren().empty())
  {
    return true;
  }

  auto first_child = array.GetChildren().at(0);
  return first_child->GetAnyTypeName() == scalar_type;
}

std::vector<std::string> GetAnyValueItemTypes()
{
  return {AnyValueEmptyItem::Type, AnyValueScalarItem::Type, AnyValueStructItem::Type,
          AnyValueArrayItem::Type};
}

mvvm::TagInfo CreateAnyValueTag(std::string name, const std::optional<size_t> &min,
                                const std::optional<size_t> &max)
{
  return {std::move(name), min, max, GetAnyValueItemTypes()};
}

}  // namespace sup::gui
