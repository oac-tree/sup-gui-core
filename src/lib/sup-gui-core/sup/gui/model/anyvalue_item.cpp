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

#include "anyvalue_item.h"

#include "anyvalue_item_constants.h"
#include "anyvalue_item_utils.h"
#include "scalar_conversion_utils.h"
#include "scalartype_property_item.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <mvvm/model/item_utils.h>

namespace sup::gui
{

// ----------------------------------------------------------------------------
// AnyValueItem
// ----------------------------------------------------------------------------

AnyValueItem::AnyValueItem(const std::string& item_type) : CompoundItem(item_type)
{
  (void)SetFlag(mvvm::Appearance::kProperty, true);
}

std::unique_ptr<mvvm::SessionItem> AnyValueItem::Clone() const
{
  // This base is not intended to be used directly
  throw sup::gui::NotImplementedException("Clone for AnyValueItem is not implemented");
}

void AnyValueItem::SetAnyTypeName(const std::string& type_name)
{
  SetProperty(constants::kAnyValueTypeTag, type_name);
}

std::string AnyValueItem::GetAnyTypeName() const
{
  return Property<std::string>(constants::kAnyValueTypeTag);
}

bool AnyValueItem::IsScalar() const
{
  return false;
}

bool AnyValueItem::IsStruct() const
{
  return false;
}

bool AnyValueItem::IsArray() const
{
  return false;
}

std::vector<AnyValueItem*> AnyValueItem::GetChildren() const
{
  return {};
}

int AnyValueItem::GetChildrenCount() const
{
  return static_cast<int>(GetChildren().size());
}

// ----------------------------------------------------------------------------
// AnyValueEmptyItem
// ----------------------------------------------------------------------------

AnyValueEmptyItem::AnyValueEmptyItem() : AnyValueItem(GetStaticType())
{
  (void)SetDisplayName(constants::kEmptyTypeName);
  (void)SetToolTip(constants::kEmptyTypeName);
  AddProperty(constants::kAnyValueTypeTag, constants::kEmptyTypeName)
      .SetEditable(false)
      .SetVisible(false);
}

std::string AnyValueEmptyItem::GetStaticType()
{
  return "AnyValueEmpty";
}

std::unique_ptr<mvvm::SessionItem> AnyValueEmptyItem::Clone() const
{
  return std::make_unique<AnyValueEmptyItem>(*this);
}

// ----------------------------------------------------------------------------
// AnyValueScalarItem
// ----------------------------------------------------------------------------

AnyValueScalarItem::AnyValueScalarItem() : AnyValueItem(GetStaticType())
{
  (void)SetDisplayName(constants::kScalarTypeName);
  (void)SetToolTip(constants::kScalarTypeName);
  AddProperty<ScalarTypePropertyItem>(constants::kAnyValueTypeTag).SetVisible(false);
}

std::string AnyValueScalarItem::GetStaticType()
{
  return "AnyValueScalar";
}

std::unique_ptr<mvvm::SessionItem> AnyValueScalarItem::Clone() const
{
  return std::make_unique<AnyValueScalarItem>(*this);
}

void AnyValueScalarItem::SetAnyTypeName(const std::string& type_name)
{
  GetItem<ScalarTypePropertyItem>(constants::kAnyValueTypeTag)->SetScalarTypeName(type_name);
}

std::string AnyValueScalarItem::GetAnyTypeName() const
{
  return GetItem<ScalarTypePropertyItem>(constants::kAnyValueTypeTag)->GetScalarTypeName();
}

bool AnyValueScalarItem::IsScalar() const
{
  return true;
}

// ----------------------------------------------------------------------------
// AnyValueStructItem
// ----------------------------------------------------------------------------

AnyValueStructItem::AnyValueStructItem() : AnyValueItem(GetStaticType())
{
  (void)SetDisplayName(constants::kStructTypeName);
  (void)SetToolTip(constants::kStructTypeName);
  (void)AddProperty(constants::kAnyValueTypeTag, std::string()).SetVisible(false);
  RegisterTag(CreateAnyValueTag(constants::kAnyValueChildrenTag), /*as_default*/ true);
}

std::string AnyValueStructItem::GetStaticType()
{
  return "AnyValueStruct";
}

std::unique_ptr<mvvm::SessionItem> AnyValueStructItem::Clone() const
{
  return std::make_unique<AnyValueStructItem>(*this);
}

bool AnyValueStructItem::IsStruct() const
{
  return true;
}

AnyValueScalarItem* AnyValueStructItem::AddScalarField(const std::string& field_name,
                                                       const std::string& field_type,
                                                       const mvvm::variant_t& value)
{
  auto child = std::make_unique<AnyValueScalarItem>();
  child->SetAnyTypeName(field_type);
  child->SetData(value);
  child->SetDisplayName(field_name);
  auto child_ptr = child.get();
  (void)InsertItem(std::move(child), mvvm::TagIndex::Append());
  return child_ptr;
}

std::vector<AnyValueItem*> AnyValueStructItem::GetChildren() const
{
  return GetItems<AnyValueItem>(constants::kAnyValueChildrenTag);
}

// ----------------------------------------------------------------------------
// AnyValueArrayItem
// ----------------------------------------------------------------------------

AnyValueArrayItem::AnyValueArrayItem() : AnyValueItem(GetStaticType())
{
  (void)SetDisplayName(constants::kArrayTypeName);
  (void)SetToolTip(constants::kArrayTypeName);
  (void)AddProperty(constants::kAnyValueTypeTag, std::string()).SetVisible(false);
  RegisterTag(CreateAnyValueTag(constants::kAnyValueChildrenTag), /*as_default*/ true);
}

std::string AnyValueArrayItem::GetStaticType()
{
  return "AnyValueArray";
}

std::unique_ptr<mvvm::SessionItem> AnyValueArrayItem::Clone() const
{
  return std::make_unique<AnyValueArrayItem>(*this);
}

bool AnyValueArrayItem::IsArray() const
{
  return true;
}

std::vector<AnyValueItem*> AnyValueArrayItem::GetChildren() const
{
  return GetItems<AnyValueItem>(constants::kAnyValueChildrenTag);
}

}  // namespace sup::gui
