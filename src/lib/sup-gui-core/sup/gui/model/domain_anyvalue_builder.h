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

#ifndef SUP_GUI_MODEL_DOMAIN_ANYVALUE_BUILDER_H_
#define SUP_GUI_MODEL_DOMAIN_ANYVALUE_BUILDER_H_

#include <sup/gui/core/dto_types_fwd.h>

#include <memory>

namespace sup::gui
{

class AnyValueItem;

//! The builder to create AnyValue from AnyValueItem.

class DomainAnyValueBuilder
{
public:
  explicit DomainAnyValueBuilder(const AnyValueItem& item);
  ~DomainAnyValueBuilder();

  sup::dto::AnyValue GetAnyValue();

private:
  struct DomainAnyValueBuilderImpl;
  std::unique_ptr<DomainAnyValueBuilderImpl> p_impl;
};

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_DOMAIN_ANYVALUE_BUILDER_H_
