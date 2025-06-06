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

#ifndef SUP_GUI_MODEL_ANYVALUE_ITEM_BUILDER_H_
#define SUP_GUI_MODEL_ANYVALUE_ITEM_BUILDER_H_

#include <sup/gui/core/dto_types_fwd.h>

#include <sup/dto/i_any_visitor.h>

#include <memory>

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{

class AnyValueItem;

class AnyValueItemBuilder : public sup::dto::IAnyVisitor<const sup::dto::AnyValue>
{
public:
  std::unique_ptr<AnyValueItem> MoveAnyValueItem();

  void EmptyProlog(const anyvalue_t* anyvalue) override;
  void EmptyEpilog(const anyvalue_t* anyvalue) override;

  void StructProlog(const anyvalue_t* anyvalue) override;
  void StructMemberSeparator() override;
  void StructEpilog(const anyvalue_t* anyvalue) override;

  void MemberProlog(const anyvalue_t* anyvalue, const std::string& member_name) override;
  void MemberEpilog(const anyvalue_t* anyvalue, const std::string& member_name) override;

  void ArrayProlog(const anyvalue_t* anyvalue) override;
  void ArrayElementSeparator() override;
  void ArrayEpilog(const anyvalue_t* anyvalue) override;

  void ScalarProlog(const anyvalue_t* anyvalue) override;
  void ScalarEpilog(const anyvalue_t* anyvalue) override;

private:
  void AddItem(std::unique_ptr<AnyValueItem> item);

  std::unique_ptr<AnyValueItem> m_result;
  mvvm::SessionItem* m_current_item{nullptr};
  int m_index{-1};
  std::string m_member_name;
};

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_ANYVALUE_ITEM_BUILDER_H_
