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

#include "sup/gui/components/custom_presentation_items.h"

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/core/variant.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/combo_property.h>
#include <mvvm/providers/custom_variants.h>

#include <gtest/gtest.h>

#include <QColor>

namespace sup::gui::test
{

//! Testing presentation items from custom_presentation_items.h

class CustomPresentationItemsTest : public ::testing::Test
{
};

//! Integer scalar is shown as an editable cell.
TEST_F(CustomPresentationItemsTest, ScalarValueInteger)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);
  (void)item.SetData(mvvm::int32{42});

  AnyValueScalarValuePresentationItem presentation(&item);

  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_TRUE(presentation.IsEditable());
  EXPECT_FALSE(presentation.IsCheckable());
  EXPECT_EQ(presentation.Data(Qt::DisplayRole), QString("42"));
  EXPECT_EQ(presentation.Data(Qt::EditRole).toInt(), 42);
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());

  EXPECT_TRUE(presentation.SetData(QVariant::fromValue(mvvm::int32{43}), Qt::EditRole));
  EXPECT_EQ(item.Data<mvvm::int32>(), 43);
}

//! Boolean scalar is shown as a checkbox.
TEST_F(CustomPresentationItemsTest, ScalarValueBoolean)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kBooleanTypeName);
  (void)item.SetData(true);

  AnyValueScalarValuePresentationItem presentation(&item);

  EXPECT_FALSE(presentation.IsEditable());
  EXPECT_TRUE(presentation.IsCheckable());
  EXPECT_EQ(presentation.Data(Qt::DisplayRole), QString("True"));
  EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Checked);

  EXPECT_TRUE(presentation.SetData(Qt::Unchecked, Qt::CheckStateRole));
  EXPECT_FALSE(item.Data<bool>());
  EXPECT_FALSE(item.HasData(mvvm::DataRole::kCheckState));
}

//! The presentation becomes a checkbox after the type change from int32 to bool.
TEST_F(CustomPresentationItemsTest, ScalarValueChangeIntegerToBoolean)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);

  AnyValueScalarValuePresentationItem presentation(&item);
  EXPECT_FALSE(presentation.IsCheckable());

  item.SetAnyTypeName(mvvm::constants::kBooleanTypeName);

  EXPECT_FALSE(presentation.IsEditable());
  EXPECT_TRUE(presentation.IsCheckable());
  EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);

  EXPECT_TRUE(presentation.SetData(Qt::Checked, Qt::CheckStateRole));
  EXPECT_TRUE(item.Data<bool>());
  EXPECT_FALSE(item.HasData(mvvm::DataRole::kCheckState));
}

//! The presentation becomes an editable cell after the type change from bool to int32.
TEST_F(CustomPresentationItemsTest, ScalarValueChangeBooleanToInteger)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kBooleanTypeName);

  AnyValueScalarValuePresentationItem presentation(&item);
  EXPECT_FALSE(presentation.IsEditable());

  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);

  EXPECT_TRUE(presentation.IsEditable());
  EXPECT_FALSE(presentation.IsCheckable());
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());

  EXPECT_TRUE(presentation.SetData(QVariant::fromValue(mvvm::int32{42}), Qt::EditRole));
  EXPECT_EQ(item.Data<mvvm::int32>(), 42);
}

//! Presentation-only role is kept for any data type and survives the type change.
TEST_F(CustomPresentationItemsTest, ScalarValuePresentationOnlyRole)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kBooleanTypeName);

  AnyValueScalarValuePresentationItem presentation(&item);
  EXPECT_TRUE(presentation.IsPresentationOnlyRole(Qt::BackgroundRole));

  EXPECT_TRUE(presentation.SetData(QColor(Qt::red), Qt::BackgroundRole));
  EXPECT_EQ(presentation.Data(Qt::BackgroundRole), QColor(Qt::red));

  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);
  EXPECT_EQ(presentation.Data(Qt::BackgroundRole), QColor(Qt::red));

  // invalid variant resets the role
  EXPECT_TRUE(presentation.SetData(QVariant(), Qt::BackgroundRole));
  EXPECT_FALSE(presentation.Data(Qt::BackgroundRole).isValid());
}

//! Data change emits the check state, since the checkbox can appear or disappear.
TEST_F(CustomPresentationItemsTest, ScalarValueQtRolesToEmit)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);

  const AnyValueScalarValuePresentationItem presentation(&item);

  const QVector<mvvm::role_t> expected_roles{Qt::DisplayRole, Qt::EditRole, Qt::CheckStateRole};
  EXPECT_EQ(presentation.GetQtRolesToEmit(mvvm::DataRole::kData), expected_roles);
}

//! Type cell shows the type name and offers the combo with scalar types.
TEST_F(CustomPresentationItemsTest, ScalarTypeInitialState)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);

  const AnyValueScalarTypePresentationItem presentation(&item);

  const auto expected_combo =
      mvvm::ComboProperty::CreateFrom(GetScalarTypeNames(), mvvm::constants::kInt32TypeName);

  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_TRUE(presentation.IsEditable());
  EXPECT_FALSE(presentation.IsCheckable());
  EXPECT_EQ(presentation.Data(Qt::DisplayRole), QString("int32"));
  EXPECT_EQ(presentation.Data(Qt::EditRole).value<mvvm::ComboProperty>(), expected_combo);
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());
}

//! Boolean scalar has no checkbox in the type cell.
TEST_F(CustomPresentationItemsTest, ScalarTypeBoolean)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kBooleanTypeName);

  AnyValueScalarTypePresentationItem presentation(&item);

  EXPECT_FALSE(presentation.IsCheckable());
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());
  EXPECT_FALSE(presentation.SetData(Qt::Checked, Qt::CheckStateRole));
  EXPECT_FALSE(item.HasData(mvvm::DataRole::kCheckState));
}

//! Scalar without type has nothing to edit in the type cell.
TEST_F(CustomPresentationItemsTest, ScalarTypeUndefined)
{
  AnyValueScalarItem item;

  const AnyValueScalarTypePresentationItem presentation(&item);

  EXPECT_FALSE(presentation.IsEditable());
  EXPECT_EQ(presentation.Data(Qt::DisplayRole), QString());
  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());
}

//! New combo selection changes the scalar type and resets the value.
TEST_F(CustomPresentationItemsTest, ScalarTypeChange)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);
  (void)item.SetData(mvvm::int32{42});

  AnyValueScalarTypePresentationItem presentation(&item);

  auto combo = presentation.Data(Qt::EditRole).value<mvvm::ComboProperty>();
  combo.SetValue(mvvm::constants::kStringTypeName);
  EXPECT_TRUE(presentation.SetData(QVariant::fromValue(combo), Qt::EditRole));

  EXPECT_EQ(item.GetAnyTypeName(), mvvm::constants::kStringTypeName);
  EXPECT_EQ(item.Data<std::string>(), std::string());
  EXPECT_EQ(presentation.Data(Qt::DisplayRole), QString("string"));

  // not a combo
  EXPECT_FALSE(presentation.SetData(QString("int8"), Qt::EditRole));
  EXPECT_EQ(item.GetAnyTypeName(), mvvm::constants::kStringTypeName);
}

//! Selection of the current type keeps the value.
TEST_F(CustomPresentationItemsTest, ScalarTypeSameSelection)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);
  (void)item.SetData(mvvm::int32{42});

  AnyValueScalarTypePresentationItem presentation(&item);

  EXPECT_FALSE(presentation.SetData(presentation.Data(Qt::EditRole), Qt::EditRole));
  EXPECT_EQ(item.Data<mvvm::int32>(), 42);
}

//! Type cell is not editable when the scalar is not.
TEST_F(CustomPresentationItemsTest, ScalarTypeReadOnlyScalar)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);

  const AnyValueScalarTypePresentationItem presentation(&item);
  EXPECT_TRUE(presentation.IsEditable());

  (void)item.SetEditable(false);
  EXPECT_FALSE(presentation.IsEditable());
}

//! Type cell keeps presentation-only roles, e.g. the background set by the diff highlight.
TEST_F(CustomPresentationItemsTest, ScalarTypePresentationOnlyRole)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(mvvm::constants::kInt32TypeName);

  AnyValueScalarTypePresentationItem presentation(&item);

  EXPECT_TRUE(presentation.SetData(QColor(Qt::red), Qt::BackgroundRole));
  EXPECT_EQ(presentation.Data(Qt::BackgroundRole), QColor(Qt::red));
}

//! Type change through the type cell is one undo step.
TEST_F(CustomPresentationItemsTest, ScalarTypeChangeUndo)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<AnyValueScalarItem>();
  item->SetAnyTypeName(mvvm::constants::kInt8TypeName);
  (void)item->SetData(mvvm::int8{42});

  model.SetUndoEnabled(true);
  auto commands = model.GetCommandStack();

  AnyValueScalarTypePresentationItem presentation(item);
  auto combo = presentation.Data(Qt::EditRole).value<mvvm::ComboProperty>();
  combo.SetValue(mvvm::constants::kBooleanTypeName);
  EXPECT_TRUE(presentation.SetData(QVariant::fromValue(combo), Qt::EditRole));
  EXPECT_EQ(item->GetAnyTypeName(), mvvm::constants::kBooleanTypeName);
  EXPECT_EQ(commands->GetCommandCount(), 1U);

  commands->Undo();
  EXPECT_EQ(item->GetAnyTypeName(), mvvm::constants::kInt8TypeName);
  EXPECT_EQ(item->Data<mvvm::int8>(), mvvm::int8{42});
}

}  // namespace sup::gui::test
