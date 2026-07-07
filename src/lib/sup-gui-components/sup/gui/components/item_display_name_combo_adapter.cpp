/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "item_display_name_combo_adapter.h"

#include <sup/gui/model/configuration_item.h>

#include <mvvm/utils/container_utils.h>

namespace sup::gui
{



QList<QString> ItemDisplayNameComboAdapter::GetAvailableItemNames() const
{
  QList<QString> result{kNoSelection};
  for (const auto& item : m_items)
  {
    result.append(QString::fromStdString(item->GetDisplayName()));
  }
  return result;
}

mvvm::SessionItem* ItemDisplayNameComboAdapter::GetItemFromComboIndex(std::int32_t combo_index)
{
  // first index means "no selection"
  if (combo_index < 1)
  {
    return nullptr;
  }

  const auto item_index = combo_index - 1;
  if (item_index >= static_cast<int32_t>(m_items.size()))
  {
    return nullptr;
  }

  return m_items.at(static_cast<std::size_t>(item_index));
}

std::int32_t ItemDisplayNameComboAdapter::GetComboIndexFromItem(const mvvm::SessionItem* item)
{
  if (item == nullptr)
  {
    return 0;
  }

  const auto index = mvvm::utils::IndexOfItem(m_items, item);
  return index == mvvm::constants::kInvalidIndex ? 0 : static_cast<std::int32_t>(index) + 1;
}

}  // namespace sup::gui
