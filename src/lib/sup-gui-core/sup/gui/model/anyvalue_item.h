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

#ifndef SUP_GUI_MODEL_ANYVALUE_ITEM_H_
#define SUP_GUI_MODEL_ANYVALUE_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace sup::gui
{

/**
 * @brief The AnyValueItem class is a base for all AnyValueItems.
 *
 * It's a counterpart of sup::dto::AnyValue in the GUI world. It is intended to show structured
 * data in Qt's trees and tables.
 */
class AnyValueItem : public mvvm::CompoundItem
{
public:
  explicit AnyValueItem(const std::string& item_type);

  std::unique_ptr<SessionItem> Clone() const override;

  virtual void SetAnyTypeName(const std::string& type_name);

  virtual std::string GetAnyTypeName() const;

  virtual bool IsScalar() const;
  virtual bool IsStruct() const;
  virtual bool IsArray() const;

  virtual std::vector<AnyValueItem*> GetChildren() const;

  int GetChildrenCount() const;
};

/**
 * @brief The AnyValueEmptyItem represents empty AnyValue.
 */
class AnyValueEmptyItem : public AnyValueItem
{
public:
  AnyValueEmptyItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;
};

/**
 * @brief The AnyValueScalarItem class represents AnyValue scalar.
 */
class AnyValueScalarItem : public AnyValueItem
{
public:
  AnyValueScalarItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  void SetAnyTypeName(const std::string& type_name) override;

  std::string GetAnyTypeName() const override;

  bool IsScalar() const override;
};

/**
 * @brief The AnyValueStructItem class represents AnyValue structure.
 */
class AnyValueStructItem : public AnyValueItem
{
public:
  AnyValueStructItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  bool IsStruct() const override;

  AnyValueScalarItem* AddScalarField(const std::string& field_name, const std::string& field_type,
                                     const mvvm::variant_t& value);

  std::vector<AnyValueItem*> GetChildren() const override;
};

/**
 * @brief The AnyValueArrayItem class represents an AnyValue array.
 */
class AnyValueArrayItem : public AnyValueItem
{
public:
  AnyValueArrayItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  bool IsArray() const override;

  std::vector<AnyValueItem*> GetChildren() const override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_ANYVALUE_ITEM_H_
