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

std::vector<mvvm::SessionItem *> GetTopLevelSelection(const std::vector<mvvm::SessionItem *> &items)
{
  std::vector<std::pair<mvvm::SessionItem *, int>> item_to_depth;

  int min_depth = std::numeric_limits<int>::max();
  auto on_element = [&min_depth](auto element) -> std::pair<mvvm::SessionItem *, int>
  {
    int depth = mvvm::utils::GetNestingDepth(element);
    if (depth >= 0)
    {
      min_depth = std::min(min_depth, depth);
    }
    return {element, depth};
  };
  std::transform(items.begin(), items.end(), std::back_inserter(item_to_depth), on_element);

  std::vector<mvvm::SessionItem *> result;

  auto on_pair_element = [min_depth, &result](auto element)
  {
    if (element.second == min_depth)
    {
      result.push_back(element.first);
    }
  };
  std::for_each(item_to_depth.begin(), item_to_depth.end(), on_pair_element);

  return result;
}

}  // namespace sup::gui
