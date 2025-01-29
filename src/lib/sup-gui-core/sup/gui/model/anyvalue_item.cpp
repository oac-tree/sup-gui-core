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

#include "anyvalue_item.h"

#include "anyvalue_item_constants.h"
#include "anyvalue_item_utils.h"
#include "scalar_conversion_utils.h"

#include <sup/gui/core/exceptions.h>

namespace sup::gui
{

// ----------------------------------------------------------------------------
// AnyValueItem
// ----------------------------------------------------------------------------

AnyValueItem::AnyValueItem(const std::string& item_type) : CompoundItem(item_type)
{
  SetFlag(mvvm::Appearance::kProperty, true);
}

std::unique_ptr<mvvm::SessionItem> AnyValueItem::Clone() const
{
  // This base is not intended to be used directly
  throw sup::gui::NotImplementedException("Clone for AnyValueItem is not implemented");
}

void AnyValueItem::SetAnyTypeName(const std::string& type_name)
{
  SetData(type_name, constants::kAnyTypeNameRole);
}

std::string AnyValueItem::GetAnyTypeName() const
{
  return HasData(constants::kAnyTypeNameRole) ? Data<std::string>(constants::kAnyTypeNameRole)
                                              : std::string();
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
  SetDisplayName(constants::kEmptyTypeName);
  SetAnyTypeName(constants::kEmptyTypeName);
  SetToolTip(constants::kEmptyTypeName);
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
  SetDisplayName(constants::kScalarTypeName);
  SetToolTip(constants::kScalarTypeName);
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
  AnyValueItem::SetAnyTypeName(type_name);
  // setting default value for given type
  SetData(GetVariantFromScalarTypeName(type_name));
  SetToolTip(type_name);
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
  SetDisplayName(constants::kStructTypeName);
  SetAnyTypeName("");
  SetToolTip(constants::kStructTypeName);
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
  auto child = InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  child->SetAnyTypeName(field_type);
  child->SetData(value);
  child->SetDisplayName(field_name);
  child->SetToolTip(field_type);
  return child;
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
  SetDisplayName(constants::kArrayTypeName);
  SetAnyTypeName("");
  SetToolTip(constants::kArrayTypeName);
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
