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

#include "custom_row_strategies.h"

#include "custom_presentation_items.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <mvvm/providers/viewitem.h>
#include <mvvm/providers/viewitem_factory.h>

#include <QColor>

namespace sup::gui
{

namespace
{

/**
 * @brief Returns true if given item should have editable display name.
 *
 * @details The name of array elements should be read only.
 */
bool HasEditableDisplayName(const AnyValueItem& item)
{
  auto item_parent = dynamic_cast<const AnyValueItem*>(item.GetParent());
  return (item_parent == nullptr) || !item_parent->IsArray();
}

}  // namespace

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
    mvvm::SessionItem* item)
{
  auto anyvalue_item = dynamic_cast<AnyValueItem*>(item);
  if (anyvalue_item == nullptr)
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

  // second and third columns
  if (auto scalar_item = dynamic_cast<AnyValueScalarItem*>(anyvalue_item); scalar_item)
  {
    (void)result.emplace_back(std::make_unique<mvvm::ViewItem>(
        std::make_unique<AnyValueScalarValuePresentationItem>(scalar_item)));
    (void)result.emplace_back(std::make_unique<mvvm::ViewItem>(
        std::make_unique<AnyValueScalarTypePresentationItem>(scalar_item)));
  }
  else
  {
    (void)result.emplace_back(mvvm::CreateDataViewItem(anyvalue_item));
    (void)result.emplace_back(
        mvvm::CreateDataViewItem(anyvalue_item->FindItem(constants::kAnyValueTypeTag)));
  }

  return result;
}

}  // namespace sup::gui
