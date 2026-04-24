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

#include "sup/gui/components/mime_conversion_helper.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/core/variant.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/standarditems/vector_item.h>

#include <gtest/gtest.h>

#include <QMimeData>

namespace sup::gui::test
{

class MimeConversionHelperTests : public ::testing::Test
{
};

TEST_F(MimeConversionHelperTests, SetStringListToMimeAndGetItBack)
{
  {  // empty mime
    const QMimeData mime_data;
    EXPECT_TRUE(GetStringListFromMime(mime_data, "").empty());
  }

  {  // single entry in mime
    const QString mime_type = "application.coa.tests";
    const QStringList expected_list({"abc"});
    QMimeData mime_data;
    SetStringListToMime(expected_list, mime_type, mime_data);
    EXPECT_EQ(GetStringListFromMime(mime_data, mime_type), expected_list);
  }

  {  // two entries
    const QString mime_type = "application.coa.tests";
    const QStringList expected_list({"abc", "def"});
    QMimeData mime_data;
    SetStringListToMime(expected_list, mime_type, mime_data);
    EXPECT_EQ(GetStringListFromMime(mime_data, mime_type), expected_list);
  }
}

TEST_F(MimeConversionHelperTests, SetItemTypesToMimeAndGetItBack)
{
  {  //  empty mime
    const QMimeData mime_data;
    EXPECT_TRUE(GetSessionItemType(mime_data, "").empty());
  }

  {  // wrong mime type
    const QMimeData mime_data;
    EXPECT_TRUE(GetSessionItemType(mime_data, "application.coa.tests").empty());
  }

  {  // single item
    AnyValueStructItem item;
    const std::vector<const mvvm::SessionItem*> items({&item});
    const QString mime_type = "application.coa.tests";
    QMimeData mime_data;
    SetItemTypesToMime(items, mime_type, mime_data);
    EXPECT_EQ(GetItemTypesFromMime(mime_data, mime_type),
              std::vector<std::string>({item.GetType()}));
  }

  {  // two items
    AnyValueStructItem item1;
    AnyValueArrayItem item2;

    const std::vector<const mvvm::SessionItem*> items({&item1, &item2});
    const QString mime_type = "application.coa.tests";
    QMimeData mime_data;
    SetItemTypesToMime(items, mime_type, mime_data);
    EXPECT_EQ(GetItemTypesFromMime(mime_data, mime_type),
              std::vector<std::string>({item1.GetType(), item2.GetType()}));
  }
}

TEST_F(MimeConversionHelperTests, SetItemIdentifiersToMimeAndGetItBack)
{
  {  // empty mime
    const QMimeData mime_data;
    EXPECT_TRUE(GetItemIdentifiersFromMime(mime_data, "").empty());
  }

  {  // wrong mime type
    const QMimeData mime_data;
    EXPECT_TRUE(GetItemIdentifiersFromMime(mime_data, "application.coa.tests").empty());
  }

  {  // single item
    AnyValueStructItem item;
    const std::vector<const mvvm::SessionItem*> items({&item});
    const QString mime_type = "application.coa.tests";
    QMimeData mime_data;
    SetItemIdentifiersToMime(items, mime_type, mime_data);
    EXPECT_EQ(GetItemIdentifiersFromMime(mime_data, mime_type),
              std::vector<std::string>({item.GetIdentifier()}));
  }

  {  // two items
    AnyValueStructItem item1;
    AnyValueArrayItem item2;

    const std::vector<const mvvm::SessionItem*> items({&item1, &item2});
    const QString mime_type = "application.coa.tests";
    QMimeData mime_data;
    SetItemIdentifiersToMime(items, mime_type, mime_data);
    EXPECT_EQ(GetItemIdentifiersFromMime(mime_data, mime_type),
              std::vector<std::string>({item1.GetIdentifier(), item2.GetIdentifier()}));
  }
}

TEST_F(MimeConversionHelperTests, CopyItemsToMimeDataAndCreateBack)
{
  {  // two independent items
    const QString mime_type = "application.coa.tests";
    AnyValueStructItem item1;
    item1.SetDisplayName("item1");
    AnyValueArrayItem item2;
    item2.SetDisplayName("item2");

    const std::vector<const mvvm::SessionItem*> items({&item1, &item2});
    QMimeData mime_data;
    CopyItemsToMimeData(items, mime_type, mime_data);

    EXPECT_TRUE(mime_data.hasFormat(mime_type));

    auto reconstructed_items = CreateSessionItemsFromMimeData(mime_data, mime_type);
    ASSERT_EQ(reconstructed_items.size(), 2);
    EXPECT_EQ(reconstructed_items.at(0)->GetDisplayName(), "item1");
    EXPECT_EQ(reconstructed_items.at(0)->GetType(), item1.GetType());
    EXPECT_EQ(reconstructed_items.at(1)->GetDisplayName(), "item2");
    EXPECT_EQ(reconstructed_items.at(1)->GetType(), item2.GetType());
  }

  {  // VectorItem
    mvvm::VectorItem item;

    item.SetXYZ(1.0, 2.0, 3.0);
    const QString mime_type = "application.coa.tests";
    QMimeData mime_data;
    CopyItemsToMimeData({&item}, mime_type, mime_data);
    EXPECT_TRUE(mime_data.hasFormat(mime_type));
    auto reconstructed_items = CreateSessionItemsFromMimeData(mime_data, mime_type);
    ASSERT_EQ(reconstructed_items.size(), 1);
    auto reconstructed_vector = dynamic_cast<mvvm::VectorItem*>(reconstructed_items.at(0).get());
    ASSERT_NE(reconstructed_vector, nullptr);
    EXPECT_DOUBLE_EQ(reconstructed_vector->X(), 1.0);
    EXPECT_DOUBLE_EQ(reconstructed_vector->Y(), 2.0);
    EXPECT_DOUBLE_EQ(reconstructed_vector->Z(), 3.0);
  }
}

TEST_F(MimeConversionHelperTests, CopyFilteredItemsToMimeDataAndCreateBack)
{
  const QString mime_type = "application.coa.tests";
  const std::string expected_name1("abc");
  const std::string expected_name2("def");

  mvvm::CompoundItem compound;
  compound.SetDisplayName(expected_name1);
  compound.RegisterTag(mvvm::TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/
                       true);

  auto child0 =
      compound.InsertItem(std::make_unique<mvvm::PropertyItem>(), mvvm::TagIndex::Append());
  child0->SetDisplayName("child_name0");
  auto child1 =
      compound.InsertItem(std::make_unique<mvvm::PropertyItem>(), mvvm::TagIndex::Append());
  child1->SetDisplayName("child_name1");

  auto item_accept_func = [](const auto& item) -> bool
  { return item.GetDisplayName() != "child_name0"; };

  QMimeData mime_data;
  CopyItemsToMimeData({&compound}, mime_type, mime_data, item_accept_func);

  auto reconstructed_items = CreateSessionItemsFromMimeData(mime_data, mime_type);
  ASSERT_EQ(reconstructed_items.size(), 1);

  auto reconstructed_compound = reconstructed_items.at(0).get();
  EXPECT_EQ(reconstructed_compound->GetItemCount("defaultTag"), 1);
  EXPECT_EQ(reconstructed_compound->GetItem("defaultTag")->GetDisplayName(),
            std::string("child_name1"));
}

// ===============================================================================================
// Will be refactored section
// ===============================================================================================

TEST_F(MimeConversionHelperTests, CreateCopyMimeData)
{
  const QString mime_type = "application.coa.tests";

  {  // empty list
    auto mime_data = CreateCopyMimeData(std::vector<const mvvm::SessionItem*>{}, QString());
    EXPECT_EQ(mime_data.get(), nullptr);
  }

  {  // two items
    AnyValueStructItem item1;
    AnyValueArrayItem item2;

    const std::vector<const mvvm::SessionItem*> items({&item1, &item2});

    auto mime_data = CreateCopyMimeData(items, mime_type);
    // current implementation returns type of the first element only
    EXPECT_EQ(GetSessionItemType(*mime_data, mime_type), item1.GetType());
  }

  const AnyValueStructItem item;
  auto mime_data = CreateCopyMimeData(item, mime_type);
  EXPECT_EQ(GetSessionItemType(*mime_data, mime_type), item.GetType());
}

TEST_F(MimeConversionHelperTests, GetSessionItemType)
{
  const QString mime_type = "application.coa.tests";
  const AnyValueStructItem item;
  auto mime_data = CreateCopyMimeData(item, mime_type);
  EXPECT_EQ(GetSessionItemType(*mime_data, mime_type), item.GetType());
}

TEST_F(MimeConversionHelperTests, CreatePropertyFromMime)
{
  const QString mime_type = "application.coa.tests";
  {
    // wrong mime type
    const QMimeData data;
    auto item = CreateSessionItem(data, mime_type);
    EXPECT_EQ(item.get(), nullptr);
  }

  {
    const std::string expected_name("abc");
    mvvm::PropertyItem item;
    item.SetDisplayName(expected_name);
    item.SetData(42);

    auto data = CreateCopyMimeData(item, mime_type);
    EXPECT_TRUE(data->hasFormat(mime_type));

    auto reconstructed_item = CreateSessionItem(*data, mime_type);
    EXPECT_EQ(reconstructed_item->GetDisplayName(), expected_name);

    auto reconstructed_property = dynamic_cast<mvvm::PropertyItem*>(reconstructed_item.get());
    ASSERT_NE(reconstructed_property, nullptr);
    EXPECT_EQ(reconstructed_property->Data<int>(), 42);
  }
}

TEST_F(MimeConversionHelperTests, CreateTwoItemsFromMime)
{
  const QString mime_type = "application.coa.tests";
  const std::string expected_name1("abc");
  const std::string expected_name2("def");

  mvvm::PropertyItem property;
  property.SetDisplayName(expected_name1);
  property.SetData(42);

  mvvm::CompoundItem compound;
  compound.SetDisplayName(expected_name2);

  auto data = CreateCopyMimeData({&property, &compound}, mime_type);
  EXPECT_TRUE(data->hasFormat(mime_type));

  auto reconstructed_items = CreateSessionItems(*data, mime_type);
  ASSERT_EQ(reconstructed_items.size(), 2);

  auto reconstructed_property = reconstructed_items.at(0).get();
  auto reconstructed_compound = reconstructed_items.at(1).get();

  EXPECT_EQ(reconstructed_property->GetDisplayName(), expected_name1);
  EXPECT_EQ(reconstructed_property->GetType(), mvvm::GetTypeName<mvvm::PropertyItem>());
  EXPECT_EQ(reconstructed_property->Data<int>(), 42);
  EXPECT_EQ(reconstructed_compound->GetDisplayName(), expected_name2);
  EXPECT_EQ(reconstructed_compound->GetType(), mvvm::GetTypeName<mvvm::CompoundItem>());
}

TEST_F(MimeConversionHelperTests, CopyAndPasteWithFiltering)
{
  const QString mime_type = "application.coa.tests";
  const std::string expected_name1("abc");
  const std::string expected_name2("def");

  mvvm::CompoundItem compound;
  compound.SetDisplayName(expected_name1);
  compound.RegisterTag(mvvm::TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/
                       true);

  auto child0 =
      compound.InsertItem(std::make_unique<mvvm::PropertyItem>(), mvvm::TagIndex::Append());
  child0->SetDisplayName("child_name0");
  auto child1 =
      compound.InsertItem(std::make_unique<mvvm::PropertyItem>(), mvvm::TagIndex::Append());
  child1->SetDisplayName("child_name1");

  auto item_accept_func = [](const auto& item) -> bool
  { return item.GetDisplayName() != "child_name0"; };

  auto data = CreateCopyMimeData(compound, mime_type, item_accept_func);

  auto reconstructed_items = CreateSessionItems(*data, mime_type);
  ASSERT_EQ(reconstructed_items.size(), 1);

  auto reconstructed_compound = reconstructed_items.at(0).get();
  EXPECT_EQ(reconstructed_compound->GetItemCount("defaultTag"), 1);
  EXPECT_EQ(reconstructed_compound->GetItem("defaultTag")->GetDisplayName(),
            std::string("child_name1"));
}

TEST_F(MimeConversionHelperTests, CreateItemSelectionCopyMimeData)
{
  const QString mime_type = "application.coa.tests";
  mvvm::SessionModel model;

  auto parent0 = model.InsertItem<AnyValueStructItem>();
  parent0->SetAnyTypeName("abc");
  auto child0 = parent0->AddScalarField("field0", mvvm::int32{42});
  auto child1 = parent0->AddScalarField("field1", mvvm::boolean{true});
  auto parent1 = model.InsertItem<AnyValueStructItem>();
  parent1->SetAnyTypeName("def");

  auto data = CreateItemSelectionCopyMimeData({parent0, child0, parent1}, mime_type);
  EXPECT_TRUE(data->hasFormat(mime_type));

  auto reconstructed_items = CreateSessionItems(*data, mime_type);
  ASSERT_EQ(reconstructed_items.size(), 2);

  auto anyvalue_items =
      mvvm::utils::CastItems<AnyValueItem>(mvvm::utils::GetVectorOfPtrs(reconstructed_items));
  ASSERT_EQ(anyvalue_items.size(), 2);
  EXPECT_EQ(anyvalue_items.at(0)->GetAnyTypeName(), std::string("abc"));
  EXPECT_EQ(anyvalue_items.at(0)->GetChildrenCount(), 2);
  EXPECT_EQ(anyvalue_items.at(0)->GetChildren().at(0)->GetDisplayName(), std::string("field0"));
  EXPECT_EQ(anyvalue_items.at(0)->GetChildren().at(1)->GetDisplayName(), std::string("field1"));
  EXPECT_EQ(anyvalue_items.at(1)->GetAnyTypeName(), std::string("def"));
}

TEST_F(MimeConversionHelperTests, AddItemSelectionCopyToMimeData)
{
  constexpr auto kMimeType = "kMimeType";

  mvvm::SessionModel model;

  auto parent0 = model.InsertItem<AnyValueStructItem>();
  parent0->SetAnyTypeName("abc");
  auto child0 = parent0->AddScalarField("field0", mvvm::int32{42});
  auto child1 = parent0->AddScalarField("field1", mvvm::boolean{true});
  auto parent1 = model.InsertItem<AnyValueStructItem>();
  parent1->SetAnyTypeName("def");

  QMimeData data;
  EXPECT_FALSE(data.hasFormat(kMimeType));

  AddItemSelectionCopyToMimeData({parent0, child0, parent1}, kMimeType, data);
  EXPECT_TRUE(data.hasFormat(kMimeType));

  auto reconstructed_items = CreateSessionItems(data, kMimeType);
  ASSERT_EQ(reconstructed_items.size(), 2);

  auto anyvalue_items =
      mvvm::utils::CastItems<AnyValueItem>(mvvm::utils::GetVectorOfPtrs(reconstructed_items));
  ASSERT_EQ(anyvalue_items.size(), 2);
  EXPECT_EQ(anyvalue_items.at(0)->GetAnyTypeName(), std::string("abc"));
  EXPECT_EQ(anyvalue_items.at(0)->GetChildrenCount(), 2);
  EXPECT_EQ(anyvalue_items.at(0)->GetChildren().at(0)->GetDisplayName(), std::string("field0"));
  EXPECT_EQ(anyvalue_items.at(0)->GetChildren().at(1)->GetDisplayName(), std::string("field1"));
  EXPECT_EQ(anyvalue_items.at(1)->GetAnyTypeName(), std::string("def"));
}

TEST_F(MimeConversionHelperTests, AddItemSelectionIdentifierToMimeData)
{
  constexpr auto kMimeType = "kMimeType";
  mvvm::SessionModel model;

  auto parent0 = model.InsertItem<AnyValueStructItem>();
  auto child0 = parent0->AddScalarField("field0", mvvm::int32{42});
  auto child1 = parent0->AddScalarField("field1", mvvm::boolean{true});
  auto parent1 = model.InsertItem<AnyValueStructItem>();

  QMimeData data;
  EXPECT_FALSE(data.hasFormat(kMimeType));

  AddItemSelectionIdentifierToMimeData({parent0, child0, parent1}, kMimeType, data);
  EXPECT_TRUE(data.hasFormat(kMimeType));

  auto identifiers = GetStringListFromMime(data, kMimeType);
  ASSERT_EQ(identifiers.size(), 2);
  EXPECT_EQ(identifiers.at(0), parent0->GetIdentifier());
  EXPECT_EQ(identifiers.at(1), parent1->GetIdentifier());
}

TEST_F(MimeConversionHelperTests, CreateItemSelectionIdentifierToMimeData)
{
  constexpr auto kMimeType = "kMimeType";
  mvvm::SessionModel model;

  auto parent0 = model.InsertItem<AnyValueStructItem>();
  auto child0 = parent0->AddScalarField("field0", mvvm::int32{42});
  auto child1 = parent0->AddScalarField("field1", mvvm::boolean{true});
  auto parent1 = model.InsertItem<AnyValueStructItem>();

  auto data = CreateItemSelectionIdentifierMimeData({parent0, child0, parent1}, kMimeType);
  EXPECT_TRUE(data->hasFormat(kMimeType));

  auto identifiers = GetStringListFromMime(*data, kMimeType);
  ASSERT_EQ(identifiers.size(), 2);
  EXPECT_EQ(identifiers.at(0), parent0->GetIdentifier());
  EXPECT_EQ(identifiers.at(1), parent1->GetIdentifier());
}

}  // namespace sup::gui::test
