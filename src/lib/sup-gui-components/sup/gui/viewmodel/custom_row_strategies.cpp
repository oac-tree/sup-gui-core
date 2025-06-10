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

#include "custom_row_strategies.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodel/viewitem.h>

#include <QColor>

namespace
{

/**
 * @brief Returns true if given item should have editable display name.
 *
 * @details The name of array elements should be read only.
 */
bool HasEditableDisplayName(const sup::gui::AnyValueItem &item)
{
  auto item_parent = dynamic_cast<const sup::gui::AnyValueItem *>(item.GetParent());
  return !(item_parent && item_parent->IsArray());
}

}  // namespace

namespace sup::gui
{

std::size_t AnyValueRowStrategy::GetSize() const
{
  return 3U;
}

QStringList AnyValueRowStrategy::GetHorizontalHeaderLabels() const
{
  const static QStringList result{"Name", "Value", "TypeName"};
  return result;
}

std::vector<std::unique_ptr<mvvm::ViewItem>> AnyValueRowStrategy::ConstructRowImpl(
    mvvm::SessionItem *item)
{
  auto anyvalue_item = dynamic_cast<AnyValueItem *>(item);
  if (!anyvalue_item)
  {
    return {};
  }

  std::vector<std::unique_ptr<mvvm::ViewItem>> result;

  // first column
  if (HasEditableDisplayName(*anyvalue_item))
  {
    (void)result.emplace_back(mvvm::CreateEditableDisplayNameViewItem(anyvalue_item));
  }
  else
  {
    (void)result.emplace_back(mvvm::CreateDisplayNameViewItem(anyvalue_item));
  }

  // second column
  (void)result.emplace_back(mvvm::CreateDataViewItem(anyvalue_item));

  // third column
  (void)result.emplace_back(
      mvvm::CreateDataViewItem(anyvalue_item->GetItem(constants::kAnyValueTypeTag)));

  return result;
}

}  // namespace sup::gui
