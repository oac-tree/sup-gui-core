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

#include "scalartype_property_item.h"

#include "anyvalue_conversion_utils.h"
#include "scalar_conversion_utils.h"

#include <mvvm/model/combo_property.h>
#include <mvvm/model/item_utils.h>

namespace sup::gui
{

ScalarTypePropertyItem::ScalarTypePropertyItem() : mvvm::SessionItem(GetStaticType())
{
  SetData(mvvm::ComboProperty::CreateFrom(GetScalarTypeNames()));
}

std::unique_ptr<mvvm::SessionItem> ScalarTypePropertyItem::Clone() const
{
  return std::make_unique<ScalarTypePropertyItem>(*this);
}

std::string sup::gui::ScalarTypePropertyItem::GetStaticType()
{
  return "ScalarTypeProperty";
}

std::string ScalarTypePropertyItem::GetScalarTypeName() const
{
  return Data<mvvm::ComboProperty>().GetValue();
}

void sup::gui::ScalarTypePropertyItem::SetScalarTypeName(const std::string &type_name)
{
  auto combo_value = Data<mvvm::ComboProperty>();
  combo_value.SetValue(type_name);
  SetData(combo_value);
}

bool ScalarTypePropertyItem::SetDataInternal(const mvvm::variant_t &value, int32_t role)
{
  mvvm::utils::BeginMacro(*this, "Change scalar type");
  auto result = mvvm::SessionItem::SetDataInternal(value, role);
  auto parent = GetParent();
  if ((parent != nullptr) && (role == mvvm::DataRole::kData))
  {
    const auto type_name = GetScalarTypeName();

    if (result)
    {
      (void)mvvm::utils::ReplaceData(*parent, GetVariantFromScalarTypeName(type_name),
                                     mvvm::DataRole::kData);
    }
    else
    {
      parent->SetData(GetVariantFromScalarTypeName(type_name));
    }
    parent->SetToolTip(type_name);
  }

  mvvm::utils::EndMacro(*this);

  return result;
}

}  // namespace sup::gui
