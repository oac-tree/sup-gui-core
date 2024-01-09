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

#include "anyvalue_item.h"

#include "anyvalue_item_constants.h"
#include "anyvalue_item_utils.h"
#include "scalar_conversion_utils.h"

#include <sup/gui/core/exceptions.h>

#include <mvvm/model/item_factory.h>

namespace
{

bool RegisterAnyValueItems()
{
  mvvm::RegisterGlobalItem<sup::gui::AnyValueEmptyItem>();
  mvvm::RegisterGlobalItem<sup::gui::AnyValueScalarItem>();
  mvvm::RegisterGlobalItem<sup::gui::AnyValueStructItem>();
  mvvm::RegisterGlobalItem<sup::gui::AnyValueArrayItem>();
  return true;
}

static bool anyvalue_items_registered_flag = RegisterAnyValueItems();

}  // namespace

namespace sup::gui
{

static inline const std::string kChildren = "kChildren";

// ----------------------------------------------------------------------------
// AnyValueItem
// ----------------------------------------------------------------------------

AnyValueItem::AnyValueItem(const std::string& item_type) : CompoundItem(item_type)
{
  SetFlag(mvvm::Appearance::kProperty, true);
}

std::unique_ptr<mvvm::SessionItem> AnyValueItem::Clone(bool make_unique_id) const
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

AnyValueEmptyItem::AnyValueEmptyItem() : AnyValueItem(Type)
{
  SetDisplayName(constants::kEmptyTypeName);
  SetToolTip(constants::kEmptyTypeName);
}

std::unique_ptr<mvvm::SessionItem> AnyValueEmptyItem::Clone(bool make_unique_id) const
{
  return std::make_unique<AnyValueEmptyItem>(*this, make_unique_id);
}

// ----------------------------------------------------------------------------
// AnyValueScalarItem
// ----------------------------------------------------------------------------

AnyValueScalarItem::AnyValueScalarItem() : AnyValueItem(Type)
{
  SetDisplayName(constants::kScalarTypeName);
  SetToolTip(constants::kScalarTypeName);
}

std::unique_ptr<mvvm::SessionItem> AnyValueScalarItem::Clone(bool make_unique_id) const
{
  return std::make_unique<AnyValueScalarItem>(*this, make_unique_id);
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

AnyValueStructItem::AnyValueStructItem() : AnyValueItem(Type)
{
  SetDisplayName(constants::kStructTypeName);
  SetAnyTypeName("");
  SetToolTip(constants::kStructTypeName);
  RegisterTag(CreateAnyValueTag(kChildren), /*as_default*/ true);
}

std::unique_ptr<mvvm::SessionItem> AnyValueStructItem::Clone(bool make_unique_id) const
{
  return std::make_unique<AnyValueStructItem>(*this, make_unique_id);
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
  return GetItems<AnyValueItem>(kChildren);
}

// ----------------------------------------------------------------------------
// AnyValueArrayItem
// ----------------------------------------------------------------------------

AnyValueArrayItem::AnyValueArrayItem() : AnyValueItem(Type)
{
  SetDisplayName(constants::kArrayTypeName);
  SetAnyTypeName("");
  SetToolTip(constants::kArrayTypeName);
  RegisterTag(CreateAnyValueTag(kChildren), /*as_default*/ true);
}

std::unique_ptr<mvvm::SessionItem> AnyValueArrayItem::Clone(bool make_unique_id) const
{
  return std::make_unique<AnyValueArrayItem>(*this, make_unique_id);
}

bool AnyValueArrayItem::IsArray() const
{
  return true;
}

std::vector<AnyValueItem*> AnyValueArrayItem::GetChildren() const
{
  return GetItems<AnyValueItem>(kChildren);
}

}  // namespace sup::gui
