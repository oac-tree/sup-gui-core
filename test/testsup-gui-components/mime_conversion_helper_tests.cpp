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
    EXPECT_TRUE(GetItemTypesFromMime(mime_data, "").empty());
  }

  {  // wrong mime type
    const QMimeData mime_data;
    EXPECT_TRUE(GetItemTypesFromMime(mime_data, "application.coa.tests").empty());
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

  {  // two items
    const QString mime_type = "application.coa.tests";
    const std::string expected_name1("abc");
    const std::string expected_name2("def");

    mvvm::PropertyItem property;
    property.SetDisplayName(expected_name1);
    property.SetData(42);

    mvvm::CompoundItem compound;
    compound.SetDisplayName(expected_name2);

    QMimeData mime_data;
    CopyItemsToMimeData({&property, &compound}, mime_type, mime_data);

    EXPECT_TRUE(mime_data.hasFormat(mime_type));

    auto reconstructed_items = CreateSessionItemsFromMimeData(mime_data, mime_type);
    ASSERT_EQ(reconstructed_items.size(), 2);

    auto reconstructed_property = reconstructed_items.at(0).get();
    auto reconstructed_compound = reconstructed_items.at(1).get();

    EXPECT_EQ(reconstructed_property->GetDisplayName(), expected_name1);
    EXPECT_EQ(reconstructed_property->GetType(), mvvm::GetTypeName<mvvm::PropertyItem>());
    EXPECT_EQ(reconstructed_property->Data<int>(), 42);
    EXPECT_EQ(reconstructed_compound->GetDisplayName(), expected_name2);
    EXPECT_EQ(reconstructed_compound->GetType(), mvvm::GetTypeName<mvvm::CompoundItem>());
  }
}

TEST_F(MimeConversionHelperTests, CopyFilteredItemsToMimeDataAndCreateBack)
{
  const QString mime_type = "application.coa.tests";
  const std::string expected_name1("abc");

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

TEST_F(MimeConversionHelperTests, CreateCopyMimeData)
{
  mvvm::PropertyItem item;
  item.SetData(42);
  item.SetDisplayName("abc");

  auto mime = CreateCopyMimeData({&item}, "test_mime_type");
  EXPECT_TRUE(mime->hasFormat("test_mime_type"));
  auto reconstructed_items = CreateSessionItemsFromMimeData(*mime, "test_mime_type");
  ASSERT_EQ(reconstructed_items.size(), 1);
  auto reconstructed_item = reconstructed_items.at(0).get();
  EXPECT_EQ(reconstructed_item->GetDisplayName(), std::string("abc"));
  auto reconstructed_property = dynamic_cast<mvvm::PropertyItem*>(reconstructed_item);
  ASSERT_NE(reconstructed_property, nullptr);
  EXPECT_EQ(reconstructed_property->Data<int>(), 42);
  EXPECT_EQ(reconstructed_property->GetDisplayName(), std::string("abc"));
}

}  // namespace sup::gui::test
