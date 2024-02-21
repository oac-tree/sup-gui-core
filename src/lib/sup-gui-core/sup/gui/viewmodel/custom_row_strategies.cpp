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

#include "custom_row_strategies.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

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

/**
 * @brief Returns true if given item should have editable type name.
 *
 * @details Structs and arrays should have editable type names.
 */
bool HasEditableTypeName(const sup::gui::AnyValueItem &item)
{
  return item.IsArray() || item.IsStruct();
}
}  // namespace

namespace sup::gui
{

QStringList AnyValueRowStrategy::GetHorizontalHeaderLabels() const
{
  const static QStringList result{"Name", "Value", "TypeName"};
  return result;
}

std::vector<std::unique_ptr<mvvm::ViewItem>> AnyValueRowStrategy::ConstructRow(
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
    result.emplace_back(mvvm::CreateEditableDisplayNameViewItem(anyvalue_item));
  }
  else
  {
    result.emplace_back(mvvm::CreateDisplayNameViewItem(anyvalue_item));
  }

  // second column
  result.emplace_back(mvvm::CreateDataViewItem(anyvalue_item));

  // third column
  if (HasEditableTypeName(*anyvalue_item))
  {
    result.emplace_back(mvvm::CreateDataViewItem(anyvalue_item, constants::kAnyTypeNameRole));
  }
  else
  {
    // We want type names for all scalars to be non-editable and be shown in gray to stress that
    // they are not editable. Since type name is not a separate SessionItem, but extra role on board
    // of AnyValueItem, we have have to proceed with more complex approach:
    auto view_item = mvvm::CreateFixedDataViewItem(anyvalue_item);
    view_item->SetData(QColor(Qt::gray), Qt::ForegroundRole);
    view_item->SetData(QString::fromStdString(anyvalue_item->GetAnyTypeName()), Qt::DisplayRole);
    result.emplace_back(std::move(view_item));
  }

  return result;
}

}  // namespace sup::gui
