/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalue_viewmodel.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/factories/viewmodel_controller_factory.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/standard_row_strategies.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

#include <iostream>

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

class RowStrategy : public mvvm::RowStrategyInterface
{
public:
  QStringList GetHorizontalHeaderLabels() const override { return {"Name", "Value", "Type"}; }

  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRow(mvvm::SessionItem *item) override
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
      result.emplace_back(mvvm::CreateDataViewItem(anyvalue_item, AnyValueItem::kAnyTypeNameRole));
    }
    else
    {
      result.emplace_back(
          mvvm::CreateLabelViewItem(anyvalue_item, anyvalue_item->GetAnyTypeName()));
    }

    return result;
  }
};

AnyValueViewModel::AnyValueViewModel(mvvm::SessionModelInterface *model, QObject *parent)
    : ViewModel(parent)
{
  SetController(
      mvvm::factory::CreateController<mvvm::AllChildrenStrategy, RowStrategy>(model, this));
}

}  // namespace sup::gui
