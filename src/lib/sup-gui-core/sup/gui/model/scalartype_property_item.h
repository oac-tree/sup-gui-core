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

#ifndef SUP_GUI_MODEL_SCALARTYPE_PROPERTY_ITEM_H
#define SUP_GUI_MODEL_SCALARTYPE_PROPERTY_ITEM_H

#include <mvvm/model/session_item.h>

namespace sup::gui
{

/**
 * @brief The ScalarTypePropertyItem class is a combo property on board of AnyValueScalarItem.
 *
 * It allows to change the type of the scalar via combo box, by clicking on corresponding cell of
 * the tree or table.
 */
class ScalarTypePropertyItem : public mvvm::SessionItem
{
public:
  explicit ScalarTypePropertyItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  std::string GetScalarTypeName() const;

  void SetScalarTypeName(const std::string& type_name);
};

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_SCALARTYPE_PROPERTY_ITEM_H
