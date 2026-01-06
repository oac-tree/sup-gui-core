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

  void EmptyProlog(const sup::dto::AnyValue* anyvalue) override;
  void EmptyEpilog(const sup::dto::AnyValue* anyvalue) override;

  void StructProlog(const sup::dto::AnyValue* anyvalue) override;
  void StructMemberSeparator() override;
  void StructEpilog(const sup::dto::AnyValue* anyvalue) override;

  void MemberProlog(const sup::dto::AnyValue* anyvalue, const std::string& member_name) override;
  void MemberEpilog(const sup::dto::AnyValue* anyvalue, const std::string& member_name) override;

  void ArrayProlog(const sup::dto::AnyValue* anyvalue) override;
  void ArrayElementSeparator() override;
  void ArrayEpilog(const sup::dto::AnyValue* anyvalue) override;

  void ScalarProlog(const sup::dto::AnyValue* anyvalue) override;
  void ScalarEpilog(const sup::dto::AnyValue* anyvalue) override;

private:
  void AddItem(std::unique_ptr<AnyValueItem> item);

  std::unique_ptr<AnyValueItem> m_result;
  mvvm::SessionItem* m_current_item{nullptr};
  std::int32_t m_index{-1};
  std::string m_member_name;
};

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_ANYVALUE_ITEM_BUILDER_H_
