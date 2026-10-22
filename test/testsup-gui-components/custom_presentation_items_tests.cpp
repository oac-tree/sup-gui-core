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

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/core/variant.h>

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

}  // namespace sup::gui::test
