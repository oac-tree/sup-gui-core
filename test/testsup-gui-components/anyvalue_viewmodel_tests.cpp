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

#include "sup/gui/viewmodel/anyvalue_viewmodel.h"

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_constants.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/viewmodelbase/viewitem.h>

#include <sup/dto/anytype.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

//! Testing AnyValueViewModel class.

class AnyValueViewModelTest : public ::testing::Test
{
public:
  /**
   * @brief Test function to report if display name of the given item is editable
   */
  bool HasEditableDisplayName(const mvvm::SessionItem& item)
  {
    AnyValueViewModel viewmodel(item.GetModel());
    auto indexes = viewmodel.GetIndexOfSessionItem(&item);
    // trying to set display name to first column
    return viewmodel.setData(indexes.at(0), QVariant::fromValue(QString("aaa")), Qt::EditRole);
  }
};

//! Testing how a single scalar item looks in a view model.

TEST_F(AnyValueViewModelTest, ScalarItem)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<AnyValueScalarItem>();
  item->SetAnyTypeName(sup::dto::kInt8TypeName);
  EXPECT_EQ(item->Data<mvvm::int8>(), 0);

  AnyValueViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  auto item_displayname_index = viewmodel.index(0, 0);
  auto item_value_index = viewmodel.index(0, 1);
  auto item_type_index = viewmodel.index(0, 2);

  auto views = viewmodel.FindViews(item);
  ASSERT_EQ(views.size(), 2);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), item_displayname_index);
  EXPECT_EQ(viewmodel.indexFromItem(views[1]), item_value_index);

  views = viewmodel.FindViews(item->GetItem(constants::kAnyValueTypeTag));
  ASSERT_EQ(views.size(), 1);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), item_type_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(item_displayname_index), item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(item_value_index), item);

  EXPECT_EQ(viewmodel.data(item_displayname_index, Qt::DisplayRole).toString().toStdString(),
            constants::kScalarTypeName);
  EXPECT_EQ(viewmodel.data(item_value_index, Qt::DisplayRole).toInt(), 0);

  // it should be possible to change scalar display name
  EXPECT_TRUE(viewmodel.setData(item_displayname_index, QString("abc"), Qt::EditRole));
  EXPECT_EQ(item->GetDisplayName(), std::string("abc"));

  // it should be possible to change scalar value
  const mvvm::int8 new_value(42);
  EXPECT_TRUE(viewmodel.setData(item_value_index, QVariant::fromValue(new_value), Qt::EditRole));
  EXPECT_EQ(item->Data<mvvm::int8>(), 42);

  // FIXME enable after type item refactoring

          // // ------------------------------------------------------------------------------------------
  // // The story below is about an attempt to change scalar TypeName by clicking in a 3rd column of a
  // // viewmodel. The third column is special since we handle it with experimental
  // // FixedDataPresentationItem. It allows showing the data not connected with the original item.
  // // Here, the third column is a custom gray-colored text label coinciding with TypeName.
  // // ------------------------------------------------------------------------------------------

  // // It is not possible to change scalar type name via viewmodel. Thanks to
  // // FixedDataPresentationItem, it reports read only flags and will ignore all user attempts to
  // // interact with a cell.
  // Qt::ItemFlags expected_flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
  // EXPECT_EQ(viewmodel.flags(item_type_index), expected_flags);
  // EXPECT_EQ(views[2]->Flags(), expected_flags);

  // // However, the implementation still allows to change the label programmatically (since
  // // programmatic change bypasses Flags business).
  // EXPECT_TRUE(item->IsEditable());
  // EXPECT_TRUE(viewmodel.setData(item_type_index, QString("scalar"), Qt::EditRole));

  // // the model will see the data changed to a new value
  // EXPECT_EQ(viewmodel.data(item_type_index, Qt::EditRole), QString("scalar"));

  // // This doesn't get propagated to SessionItem, which still has original type name.
  // EXPECT_EQ(item->GetAnyTypeName(), std::string("int8"));
}

//! Testing how an empty struct item looks in a view model.

TEST_F(AnyValueViewModelTest, EmptyStructItem)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<AnyValueStructItem>();

  AnyValueViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  auto item_displayname_index = viewmodel.index(0, 0);
  auto item_value_index = viewmodel.index(0, 1);
  auto item_type_index = viewmodel.index(0, 2);

  auto views = viewmodel.FindViews(item);
  EXPECT_EQ(views.size(), 2);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), item_displayname_index);
  EXPECT_EQ(viewmodel.indexFromItem(views[1]), item_value_index);

  views = viewmodel.FindViews(item->GetItem(constants::kAnyValueTypeTag));
  EXPECT_EQ(views.size(), 1);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), item_type_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(item_displayname_index), item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(item_value_index), item);

  EXPECT_EQ(viewmodel.data(item_displayname_index, Qt::DisplayRole).toString().toStdString(),
            constants::kStructTypeName);
  EXPECT_EQ(viewmodel.data(item_value_index, Qt::DisplayRole), QVariant());

  // it should be possible to change struct display name
  EXPECT_TRUE(viewmodel.setData(item_displayname_index, QString("abc"), Qt::EditRole));
  EXPECT_EQ(item->GetDisplayName(), std::string("abc"));

  // Corner case: changing structure "value" (content of the second column) programmatically. This
  // doesn't have much sense, since the structure has empty variant_t{} as a value in a second
  // column. Empty variant doesn't have it's own editor, so it is not possible to type anything in.
  // However, the value can be changed programmatically. Leaving it under test to trace possible
  // changes in the future.
  const mvvm::int8 new_value(42);
  EXPECT_TRUE(viewmodel.setData(item_value_index, QVariant::fromValue(new_value), Qt::EditRole));
  EXPECT_EQ(item->Data<mvvm::int8>(), 42);

  // it is possible to change srtuct type name
  EXPECT_TRUE(viewmodel.setData(item_type_index, QString("my-struct"), Qt::EditRole));
  EXPECT_EQ(item->GetAnyTypeName(), std::string("my-struct"));
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
  EXPECT_EQ(viewmodel.columnCount(struct_index), 3);

  auto scalar_displayname_index = viewmodel.index(0, 0, struct_index);
  auto scalar_value_index = viewmodel.index(0, 1, struct_index);
  auto scalar_type_index = viewmodel.index(0, 2, struct_index);

  auto views = viewmodel.FindViews(scalar_item);
  ASSERT_EQ(views.size(), 2);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), scalar_displayname_index);
  EXPECT_EQ(viewmodel.indexFromItem(views[1]), scalar_value_index);

  views = viewmodel.FindViews(scalar_item->GetItem(constants::kAnyValueTypeTag));
  ASSERT_EQ(views.size(), 1);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), scalar_type_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(scalar_displayname_index), scalar_item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(scalar_value_index), scalar_item);

  EXPECT_EQ(viewmodel.data(scalar_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("scalar"));
  EXPECT_EQ(viewmodel.data(scalar_value_index, Qt::DisplayRole).toInt(), 42);
}

//! Testing how an empty array item looks in a view model.

TEST_F(AnyValueViewModelTest, EmptyArrayItem)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<AnyValueArrayItem>();

  AnyValueViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  auto item_displayname_index = viewmodel.index(0, 0);
  auto item_value_index = viewmodel.index(0, 1);
  auto item_type_index = viewmodel.index(0, 2);

  auto views = viewmodel.FindViews(item);
  ASSERT_EQ(views.size(), 2);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), item_displayname_index);
  EXPECT_EQ(viewmodel.indexFromItem(views[1]), item_value_index);

  views = viewmodel.FindViews(item->GetItem(constants::kAnyValueTypeTag));
  ASSERT_EQ(views.size(), 1);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), item_type_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(item_displayname_index), item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(item_value_index), item);

  EXPECT_EQ(viewmodel.data(item_displayname_index, Qt::DisplayRole).toString().toStdString(),
            constants::kArrayTypeName);
  EXPECT_EQ(viewmodel.data(item_value_index, Qt::DisplayRole), QVariant());

  // it should be possible to change struct display name
  EXPECT_TRUE(viewmodel.setData(item_displayname_index, QString("abc"), Qt::EditRole));
  EXPECT_EQ(item->GetDisplayName(), std::string("abc"));

  // Corner case: changing array "value" (content of the second column) programmatically. This
  // doesn't have much sense, since the array has empty variant_t{} as a value in a second
  // column. Empty variant doesn't have it's own editor, so it is not possible to type anything in.
  // However, the value can be changed programmatically. Leaving it under test to trace possible
  // changes in the future.
  mvvm::int8 new_value(42);
  EXPECT_TRUE(viewmodel.setData(item_value_index, QVariant::fromValue(new_value), Qt::EditRole));
  EXPECT_EQ(item->Data<mvvm::int8>(), 42);

  // it is possible to change array type name
  EXPECT_TRUE(viewmodel.setData(item_type_index, QString("my-array"), Qt::EditRole));
  EXPECT_EQ(item->GetAnyTypeName(), std::string("my-array"));
}

TEST_F(AnyValueViewModelTest, EditableDisplayName)
{
  // cases when display name is editable

  {  // display name of an struct is editable
    mvvm::ApplicationModel model;
    auto item = model.InsertItem<AnyValueArrayItem>();
    EXPECT_TRUE(HasEditableDisplayName(*item));
  }

  {  // display name of an array is editable
    mvvm::ApplicationModel model;
    auto item = model.InsertItem<AnyValueStructItem>();
    EXPECT_TRUE(HasEditableDisplayName(*item));
  }

  {  // display name of a scalar is editable
    mvvm::ApplicationModel model;
    auto item = model.InsertItem<AnyValueStructItem>();
    EXPECT_TRUE(HasEditableDisplayName(*item));
  }

  {  // display name of a scalar field in a struct is editable
    mvvm::ApplicationModel model;
    auto struct_item = model.InsertItem<AnyValueStructItem>();
    auto item = model.InsertItem<AnyValueScalarItem>(struct_item);
    EXPECT_TRUE(HasEditableDisplayName(*item));
  }

  {  // display name of a struct field in a struct is editable
    mvvm::ApplicationModel model;
    auto struct_item = model.InsertItem<AnyValueStructItem>();
    auto item = model.InsertItem<AnyValueStructItem>(struct_item);
    EXPECT_TRUE(HasEditableDisplayName(*item));
  }

  {  // display name of an array field in a struct is editable
    mvvm::ApplicationModel model;
    auto struct_item = model.InsertItem<AnyValueStructItem>();
    auto item = model.InsertItem<AnyValueArrayItem>(struct_item);
    EXPECT_TRUE(HasEditableDisplayName(*item));
  }

  // cases when display name is not editable

  {  // display name of an scalar field in array is not editable
    mvvm::ApplicationModel model;
    auto array_item = model.InsertItem<AnyValueArrayItem>();
    auto item = model.InsertItem<AnyValueScalarItem>(array_item);
    EXPECT_FALSE(HasEditableDisplayName(*item));
  }

  {  // display name of an array field in array is not editable
    mvvm::ApplicationModel model;
    auto array_item = model.InsertItem<AnyValueArrayItem>();
    auto item = model.InsertItem<AnyValueArrayItem>(array_item);
    EXPECT_FALSE(HasEditableDisplayName(*item));
  }

  {  // display name of an struct field in array is not editable
    mvvm::ApplicationModel model;
    auto array_item = model.InsertItem<AnyValueArrayItem>();
    auto item = model.InsertItem<AnyValueStructItem>(array_item);
    EXPECT_FALSE(HasEditableDisplayName(*item));
  }
}

//! Testing how a boolean scalar item looks in a view model.
//! There shouldn't be a CheckRole in the first column (real-life bug).

TEST_F(AnyValueViewModelTest, BooleanScalarItem)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<AnyValueScalarItem>();
  item->SetAnyTypeName(sup::dto::kBooleanTypeName);
  item->SetData(true);

  AnyValueViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  auto item_displayname_index = viewmodel.index(0, 0);
  auto item_value_index = viewmodel.index(0, 1);
  auto item_type_index = viewmodel.index(0, 2);

  EXPECT_FALSE(viewmodel.data(item_displayname_index, Qt::CheckStateRole).isValid());
  EXPECT_TRUE(viewmodel.data(item_value_index, Qt::CheckStateRole).isValid());
  EXPECT_FALSE(viewmodel.data(item_type_index, Qt::CheckStateRole).isValid());

  EXPECT_EQ(viewmodel.data(item_value_index, Qt::CheckStateRole).toInt(), Qt::Checked);
}

//! ViewModel is pointed in initially empty model. Then we insert container with AnyValueItem in the
//! container. ViewModel should recognize AnyValueItem when proper root item set (real-life bug).

TEST_F(AnyValueViewModelTest, AnyValueItemInTheContainer)
{
  mvvm::ApplicationModel model;

  AnyValueViewModel viewmodel(&model);

  auto container = model.InsertItem<mvvm::ContainerItem>();

  auto item = model.InsertItem<AnyValueScalarItem>(container, mvvm::TagIndex::Append());
  item->SetAnyTypeName(sup::dto::kInt8TypeName);
  EXPECT_EQ(item->Data<mvvm::int8>(), 0);

  EXPECT_EQ(viewmodel.rowCount(), 0);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  viewmodel.SetRootSessionItem(container);

  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 3);

  auto item_displayname_index = viewmodel.index(0, 0);
  auto item_value_index = viewmodel.index(0, 1);
  auto item_type_index = viewmodel.index(0, 2);

  EXPECT_EQ(viewmodel.data(item_displayname_index, Qt::DisplayRole).toString().toStdString(),
            constants::kScalarTypeName);
  EXPECT_EQ(viewmodel.data(item_value_index, Qt::DisplayRole).toInt(), 0);
  EXPECT_EQ(viewmodel.data(item_type_index, Qt::DisplayRole).toString().toStdString(),
            sup::dto::kInt8TypeName);
}

}  // namespace sup::gui::test
