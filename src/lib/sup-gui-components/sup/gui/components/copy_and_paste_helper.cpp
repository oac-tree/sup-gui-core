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

#include "copy_and_paste_helper.h"

#include <mvvm/model/item_utils.h>

#include <algorithm>
#include <limits>

namespace sup::gui
{

namespace
{

/**
 * @brief Creates vector of pairs containing item and its depth level in items' hierarchy.
 */
std::vector<std::pair<mvvm::SessionItem *, int>> GetItemDepthInfo(
    const std::vector<mvvm::SessionItem *> &items)
{
  std::vector<std::pair<mvvm::SessionItem *, int>> result;

  auto on_element = [](auto element) -> std::pair<mvvm::SessionItem *, int>
  { return {element, mvvm::utils::GetNestingDepth(element)}; };
  std::transform(items.begin(), items.end(), std::back_inserter(result), on_element);
  return result;
}

/**
 * @brief Returns min depth level.
 */
int GetMinDepth(const std::vector<std::pair<mvvm::SessionItem *, int>> &depth_info)
{
  int min_depth = std::numeric_limits<int>::max();
  auto on_element = [&min_depth](auto element) { min_depth = std::min(element.second, min_depth); };
  std::for_each(depth_info.begin(), depth_info.end(), on_element);
  return min_depth;
}

/**
 * @brief Returns items which have given depth in the hierarchy.
 */
std::vector<mvvm::SessionItem *> GetItemsWithDepth(
    const std::vector<std::pair<mvvm::SessionItem *, int>> &depth_info, int depth)
{
  std::vector<mvvm::SessionItem *> result;

  auto on_pair_element = [depth, &result](auto element)
  {
    if (element.second == depth)
    {
      result.push_back(element.first);
    }
  };
  std::for_each(depth_info.begin(), depth_info.end(), on_pair_element);

  return result;
}

}  // namespace

std::vector<mvvm::SessionItem *> GetTopLevelSelection(const std::vector<mvvm::SessionItem *> &items)
{
  const auto item_to_depth = GetItemDepthInfo(items);
  const int min_depth = GetMinDepth(item_to_depth);
  return GetItemsWithDepth(item_to_depth, min_depth);
}

}  // namespace sup::gui
