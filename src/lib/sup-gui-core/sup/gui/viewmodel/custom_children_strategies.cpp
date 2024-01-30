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

#include "custom_children_strategies.h"

#include <sup/gui/model/anyvalue_item_utils.h>

#include <mvvm/model/sessionitem.h>
#include <mvvm/utils/container_utils.h>

namespace sup::gui
{

std::vector<mvvm::SessionItem *> AnyValueChildrenStrategy::GetChildren(
    const mvvm::SessionItem *item) const
{
  static const std::vector<std::string> expected_types = GetAnyValueItemTypes();
  std::vector<mvvm::SessionItem *> result;
  for (auto child : item->GetAllItems())
  {
    if (mvvm::utils::Contains(expected_types, child->GetType()))
    {
      result.push_back(child);
    }
  }
  return result;
}

}  // namespace sup::gui
