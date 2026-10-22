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

#include "custom_presentation_items.h"

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/combo_property.h>
#include <mvvm/model/session_item.h>
#include <mvvm/providers/boolean_data_presentation_item.h>
#include <mvvm/providers/custom_variants.h>
#include <mvvm/providers/presentation_item_helper.h>

namespace sup::gui
{

namespace
{

//! Returns true if the item value is shown as a checkbox.
bool HasCheckBoxValue(const mvvm::SessionItem* item)
{
  return (item != nullptr) && (mvvm::GetTypeCode(item->Data()) == mvvm::TypeCode::Bool)
         && !item->HasFlag(mvvm::Appearance::kNullable);
}

//! Returns the combo with all scalar type names and the given type selected.
mvvm::ComboProperty CreateScalarTypeCombo(const std::string& type_name)
{
  static const std::vector<std::string> kScalarTypeNames = GetScalarTypeNames();
  return mvvm::ComboProperty::CreateFrom(kScalarTypeNames, type_name);
}

}  // namespace

AnyValueScalarValuePresentationItem::AnyValueScalarValuePresentationItem(AnyValueScalarItem* item)
    : mvvm::AbstractPresentationItem(item)
    , m_default_presentation(
          std::make_unique<mvvm::DefaultDataPresentationItem>(item, mvvm::DataRole::kData))
    , m_boolean_presentation(std::make_unique<mvvm::BooleanDataPresentationItem>(item))
{
}

AnyValueScalarValuePresentationItem::~AnyValueScalarValuePresentationItem() = default;

QVariant AnyValueScalarValuePresentationItem::Data(mvvm::role_t qt_role) const
{
  if (IsPresentationOnlyRole(qt_role))
  {
    return m_default_presentation->Data(qt_role);
  }

  return GetPresentation().Data(qt_role);
}

bool AnyValueScalarValuePresentationItem::SetData(const QVariant& data, mvvm::role_t qt_role)
{
  if (IsPresentationOnlyRole(qt_role))
  {
    return m_default_presentation->SetData(data, qt_role);
  }

  return GetPresentation().SetData(data, qt_role);
}

bool AnyValueScalarValuePresentationItem::IsEditable() const
{
  return GetPresentation().IsEditable();
}

bool AnyValueScalarValuePresentationItem::IsCheckable() const
{
  return GetPresentation().IsCheckable();
}

bool AnyValueScalarValuePresentationItem::IsPresentationOnlyRole(mvvm::role_t qt_role) const
{
  return m_default_presentation->IsPresentationOnlyRole(qt_role);
}

QVector<mvvm::role_t> AnyValueScalarValuePresentationItem::GetQtRolesToEmit(
    mvvm::role_t data_role) const
{
  if (data_role == mvvm::DataRole::kData)
  {
    // the check state appears or disappears on the type change
    return {mvvm::GetQtRole(Qt::DisplayRole), mvvm::GetQtRole(Qt::EditRole),
            mvvm::GetQtRole(Qt::CheckStateRole)};
  }

  return m_default_presentation->GetQtRolesToEmit(data_role);
}

const mvvm::AbstractPresentationItem& AnyValueScalarValuePresentationItem::GetPresentation() const
{
  if (HasCheckBoxValue(GetItem()))
  {
    return *m_boolean_presentation;
  }
  return *m_default_presentation;
}

mvvm::AbstractPresentationItem& AnyValueScalarValuePresentationItem::GetPresentation()
{
  if (HasCheckBoxValue(GetItem()))
  {
    return *m_boolean_presentation;
  }
  return *m_default_presentation;
}

AnyValueScalarTypePresentationItem::AnyValueScalarTypePresentationItem(AnyValueScalarItem* item)
    : mvvm::DefaultDataPresentationItem(item, mvvm::DataRole::kData), m_scalar_item(item)
{
}

QVariant AnyValueScalarTypePresentationItem::Data(mvvm::role_t qt_role) const
{
  if (m_scalar_item == nullptr)
  {
    return {};
  }

  const auto type_name = m_scalar_item->GetAnyTypeName();

  if (qt_role == mvvm::GetQtRole(Qt::DisplayRole))
  {
    return QString::fromStdString(type_name);
  }

  if (qt_role == mvvm::GetQtRole(Qt::EditRole))
  {
    return type_name.empty() ? QVariant() : QVariant::fromValue(CreateScalarTypeCombo(type_name));
  }

  if (qt_role == mvvm::GetQtRole(Qt::CheckStateRole))
  {
    return {};
  }

  return DefaultDataPresentationItem::Data(qt_role);
}

bool AnyValueScalarTypePresentationItem::SetData(const QVariant& data, mvvm::role_t qt_role)
{
  if (m_scalar_item == nullptr)
  {
    return false;
  }

  if (qt_role == mvvm::GetQtRole(Qt::EditRole))
  {
    if (!mvvm::utils::IsComboPropertyVariant(data))
    {
      return false;
    }

    const auto type_name = data.value<mvvm::ComboProperty>().GetValue();
    if (type_name == m_scalar_item->GetAnyTypeName())
    {
      return false;
    }

    m_scalar_item->SetAnyTypeName(type_name);
    return true;
  }

  if (qt_role == mvvm::GetQtRole(Qt::CheckStateRole))
  {
    return false;
  }

  return DefaultDataPresentationItem::SetData(data, qt_role);
}

bool AnyValueScalarTypePresentationItem::IsEditable() const
{
  return DefaultDataPresentationItem::IsEditable() && (m_scalar_item != nullptr)
         && !m_scalar_item->GetAnyTypeName().empty();
}

bool AnyValueScalarTypePresentationItem::IsCheckable() const
{
  return false;
}

}  // namespace sup::gui
