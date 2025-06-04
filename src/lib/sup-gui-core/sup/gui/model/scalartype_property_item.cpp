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

namespace
{

mvvm::ComboProperty GetScalarTypeCombo()
{
  static const mvvm::ComboProperty kScalarCombo =
      mvvm::ComboProperty::CreateFrom(GetScalarTypeNames());
  return kScalarCombo;
}

/**
 * @brief Update data of parent (AnyValueScalarItem) according to the given type name.
 */
void UpdateParentDataToType(const std::string& type_name, mvvm::SessionItem* parent)
{
  if (!parent || type_name.empty())
  {
    return;
  }

  (void)mvvm::utils::ReplaceData(*parent, GetVariantFromScalarTypeName(type_name),
                                 mvvm::DataRole::kData);
  parent->SetToolTip(type_name);
}

}  // namespace

ScalarTypePropertyItem::ScalarTypePropertyItem() : mvvm::SessionItem(GetStaticType())
{
  // by default it has no data and no scalar type
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
  return HasData() ? Data<mvvm::ComboProperty>().GetValue() : std::string();
}

void sup::gui::ScalarTypePropertyItem::SetScalarTypeName(const std::string& type_name)
{
  auto combo_value = GetScalarTypeCombo();
  combo_value.SetValue(type_name);
  SetData(combo_value);
}

bool ScalarTypePropertyItem::SetDataInternal(const mvvm::variant_t& value, int32_t role)
{
  mvvm::utils::BeginMacro(*this, "Change scalar type");
  auto result = mvvm::SessionItem::SetDataInternal(value, role);

  if (role == mvvm::DataRole::kData)
  {
    UpdateParentDataToType(GetScalarTypeName(), GetParent());
  }

  mvvm::utils::EndMacro(*this);

  return result;
}

}  // namespace sup::gui
