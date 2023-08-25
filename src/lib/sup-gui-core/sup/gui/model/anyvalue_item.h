/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_GUI_MODEL_ANYVALUE_ITEM_H_
#define SUP_GUI_MODEL_ANYVALUE_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace sup::gui
{

//! Base class for all AnyValueItems

class AnyValueItem : public mvvm::CompoundItem
{
public:
  using CompoundItem::CompoundItem;
  explicit AnyValueItem(const std::string& item_type);

  std::unique_ptr<SessionItem> Clone(bool make_unique_id = true) const override;

  virtual void SetAnyTypeName(const std::string& type_name);

  std::string GetAnyTypeName() const;

  virtual bool IsScalar() const;
  virtual bool IsStruct() const;
  virtual bool IsArray() const;

  virtual std::vector<AnyValueItem*> GetChildren() const;

  int GetChildrenCount() const;
};

//! The item to represent AnyValue empty item.

class AnyValueEmptyItem : public AnyValueItem
{
public:
  static inline const std::string Type = "AnyValueEmpty";

  using AnyValueItem::AnyValueItem;
  AnyValueEmptyItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id = true) const override;
};

//! The item to represent AnyValue scalar.

class AnyValueScalarItem : public AnyValueItem
{
public:
  static inline const std::string Type = "AnyValueScalar";

  using AnyValueItem::AnyValueItem;
  AnyValueScalarItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id = true) const override;

  void SetAnyTypeName(const std::string& type_name) override;

  bool IsScalar() const override;
};

//! The item to represent AnyValue struct.

class AnyValueStructItem : public AnyValueItem
{
public:
  static inline const std::string Type = "AnyValueStruct";

  using AnyValueItem::AnyValueItem;
  AnyValueStructItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id = true) const override;

  bool IsStruct() const override;

  AnyValueScalarItem* AddScalarField(const std::string& field_name, const std::string& field_type,
                                     const variant_t& value);

  std::vector<AnyValueItem*> GetChildren() const override;
};

//! The item to represent AnyValue array.

class AnyValueArrayItem : public AnyValueItem
{
public:
  static inline const std::string Type = "AnyValueArray";

  using AnyValueItem::AnyValueItem;
  AnyValueArrayItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id = true) const override;

  bool IsArray() const override;

  std::vector<AnyValueItem*> GetChildren() const override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_MODEL_ANYVALUE_ITEM_H_
