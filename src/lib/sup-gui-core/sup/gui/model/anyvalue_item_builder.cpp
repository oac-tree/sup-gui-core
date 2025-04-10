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

#include "anyvalue_item_builder.h"

#include "anyvalue_conversion_utils.h"
#include "anyvalue_item.h"
#include "anyvalue_item_constants.h"

#include <mvvm/model/session_item.h>
#include <mvvm/model/tagindex.h>

#include <sup/dto/anyvalue.h>

#include <cassert>

namespace sup::gui
{

std::unique_ptr<AnyValueItem> AnyValueItemBuilder::MoveAnyValueItem()
{
  return std::move(m_result);
}

void AnyValueItemBuilder::EmptyProlog(const anyvalue_t *anyvalue)
{
  (void)anyvalue;
  AddItem(std::make_unique<AnyValueEmptyItem>());
}

void AnyValueItemBuilder::EmptyEpilog(const anyvalue_t *anyvalue)
{
  (void)anyvalue;
}

void AnyValueItemBuilder::StructProlog(const anyvalue_t *anyvalue)
{
  auto struct_item = std::make_unique<AnyValueStructItem>();
  struct_item->SetAnyTypeName(anyvalue->GetTypeName());
  AddItem(std::move(struct_item));
}

void AnyValueItemBuilder::StructMemberSeparator() {}

void AnyValueItemBuilder::StructEpilog(const anyvalue_t *anyvalue)
{
  (void)anyvalue;
}

//! Append new child with the display name corresponding to `member_name`.
//! Update

void AnyValueItemBuilder::MemberProlog(const anyvalue_t *anyvalue, const std::string &member_name)
{
  (void)anyvalue;
  m_member_name = member_name;
}

void AnyValueItemBuilder::MemberEpilog(const anyvalue_t *anyvalue, const std::string &member_name)
{
  (void)anyvalue;
  (void)member_name;
  m_member_name.clear();
  m_current_item = m_current_item->GetParent();
}

void AnyValueItemBuilder::ArrayProlog(const anyvalue_t *anyvalue)
{
  m_index = 0;
  auto array_item = std::make_unique<AnyValueArrayItem>();
  array_item->SetAnyTypeName(anyvalue->GetTypeName());
  AddItem(std::move(array_item));
}

void AnyValueItemBuilder::ArrayElementSeparator()
{
  m_index++;
  m_current_item = m_current_item->GetParent();
}

void AnyValueItemBuilder::ArrayEpilog(const anyvalue_t *anyvalue)
{
  (void)anyvalue;
  m_index = -1;
  m_current_item = m_current_item->GetParent();
}

void AnyValueItemBuilder::ScalarProlog(const anyvalue_t *anyvalue)
{
  auto scalar = std::make_unique<AnyValueScalarItem>();
  SetDataFromScalar(*anyvalue, *scalar);

  AddItem(std::move(scalar));
}

void AnyValueItemBuilder::ScalarEpilog(const anyvalue_t *anyvalue)
{
  (void)anyvalue;
}

void AnyValueItemBuilder::AddItem(std::unique_ptr<AnyValueItem> item)
{
  auto item_ptr = item.get();

  if (!m_result)
  {
    // case when no parent structure exists
    m_result = std::move(item);
    m_current_item = item_ptr;
    return;
  }

  assert(m_current_item != nullptr);

  auto child = m_current_item->InsertItem(std::move(item), mvvm::TagIndex::Append());

  // FIXME find elegant alternative to the crap below

  // we are in member mode
  if (!m_member_name.empty())
  {
    (void) child->SetDisplayName(m_member_name);
    m_member_name.clear();
  }
  else if (m_index >= 0)
  {
    (void) child->SetDisplayName(constants::kElementNamePrefix + std::to_string(m_index));
  }
  else
  {
    throw std::logic_error("Logic error");
  }

  m_current_item = item_ptr;
}

}  // namespace sup::gui
