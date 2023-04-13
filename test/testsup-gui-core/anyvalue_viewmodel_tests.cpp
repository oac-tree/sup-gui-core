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

#include "sup/gui/viewmodel/anyvalue_viewmodel.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>

#include <sup/dto/anytype.h>

#include <gtest/gtest.h>

using namespace sup::gui;

//! Testing AnyValueViewModel class.

class AnyValueViewModelTest : public ::testing::Test
{
};

//! Testing how a single scalar item looks in a view model.

TEST_F(AnyValueViewModelTest, ScalarItem)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<AnyValueScalarItem>();
  item->SetAnyTypeName(sup::dto::kInt8TypeName);

  AnyValueViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  auto item_displayname_index = viewmodel.index(0, 0);
  auto item_value_index = viewmodel.index(0, 1);

  auto views = viewmodel.FindViews(item);
  EXPECT_EQ(views.size(), 2);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), item_displayname_index);
  EXPECT_EQ(viewmodel.indexFromItem(views[1]), item_value_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(item_displayname_index), item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(item_value_index), item);

  EXPECT_EQ(viewmodel.data(item_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("scalar"));
  EXPECT_EQ(viewmodel.data(item_value_index, Qt::DisplayRole).toInt(), 0);
}

//! Testing how an empty struct item looks in a view model.

TEST_F(AnyValueViewModelTest, EmptyStructItem)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<AnyValueStructItem>();

  AnyValueViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  auto item_displayname_index = viewmodel.index(0, 0);
  auto item_value_index = viewmodel.index(0, 1);

  auto views = viewmodel.FindViews(item);
  EXPECT_EQ(views.size(), 2);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), item_displayname_index);
  EXPECT_EQ(viewmodel.indexFromItem(views[1]), item_value_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(item_displayname_index), item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(item_value_index), item);

  EXPECT_EQ(viewmodel.data(item_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("struct"));
  EXPECT_EQ(viewmodel.data(item_value_index, Qt::DisplayRole), QVariant());
}

//! Testing how a struct with single scalar field looks in a view model.

TEST_F(AnyValueViewModelTest, StructWithScalarItem)
{
  mvvm::ApplicationModel model;
  auto struct_item = model.InsertItem<AnyValueStructItem>();
  auto scalar_item = struct_item->AddScalarField("scalar", sup::dto::kInt32TypeName, 42);

  AnyValueViewModel viewmodel(&model);

  auto struct_index = viewmodel.index(0, 0);
  EXPECT_EQ(viewmodel.rowCount(struct_index), 1);
  EXPECT_EQ(viewmodel.columnCount(struct_index), 2);

  auto scalar_displayname_index = viewmodel.index(0, 0, struct_index);
  auto scalar_value_index = viewmodel.index(0, 1, struct_index);

  auto views = viewmodel.FindViews(scalar_item);
  EXPECT_EQ(views.size(), 2);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), scalar_displayname_index);
  EXPECT_EQ(viewmodel.indexFromItem(views[1]), scalar_value_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(scalar_displayname_index), scalar_item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(scalar_value_index), scalar_item);

  EXPECT_EQ(viewmodel.data(scalar_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("scalar"));
  EXPECT_EQ(viewmodel.data(scalar_value_index, Qt::DisplayRole).toInt(), 42);
}
