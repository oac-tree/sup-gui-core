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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sup/gui/components/mime_conversion_helper.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/property_item.h>

#include <gtest/gtest.h>

#include <QMimeData>

using namespace sup::gui;

class MimeConverionHelperTests : public ::testing::Test
{
};

TEST_F(MimeConverionHelperTests, CreateCopyMimeData)
{
  {  // empty list
    auto mime_data = CreateCopyMimeData(std::vector<const mvvm::SessionItem*>{}, QString());
    EXPECT_EQ(mime_data.get(), nullptr);
  }

  {  // two items
    AnyValueStructItem item1;
    AnyValueArrayItem item2;

    const std::vector<const mvvm::SessionItem*> items({&item1, &item2});

    auto mime_data = CreateCopyMimeData(items, kCopyAnyValueMimeType);
    // current implementation returns type of the first element only
    EXPECT_EQ(GetSessionItemType(mime_data.get(), kCopyAnyValueMimeType), item1.GetType());
  }

  EXPECT_TRUE(GetSessionItemType(nullptr, QString()).empty());

  const AnyValueStructItem item;
  auto mime_data = CreateCopyMimeData(item, kCopyAnyValueMimeType);
  EXPECT_EQ(GetSessionItemType(mime_data.get(), kCopyAnyValueMimeType), item.GetType());
}

TEST_F(MimeConverionHelperTests, GetSessionItemType)
{
  EXPECT_TRUE(GetSessionItemType(nullptr, QString()).empty());

  const AnyValueStructItem item;
  auto mime_data = CreateCopyMimeData(item, kCopyAnyValueMimeType);
  EXPECT_EQ(GetSessionItemType(mime_data.get(), kCopyAnyValueMimeType), item.GetType());
}

TEST_F(MimeConverionHelperTests, CreatePropertyFromMime)
{
  const QString mime_type = "application.coa.tests";
  {
    // wrong mime type
    const QMimeData data;
    auto item = CreateSessionItem(&data, mime_type);
    EXPECT_EQ(item.get(), nullptr);
  }

  {
    const std::string expected_name("abc");
    mvvm::PropertyItem item;
    item.SetDisplayName(expected_name);
    item.SetData(42);

    auto data = CreateCopyMimeData(item, mime_type);
    EXPECT_TRUE(data->hasFormat(mime_type));

    auto reconstructed_item = CreateSessionItem(data.get(), mime_type);
    EXPECT_EQ(reconstructed_item->GetDisplayName(), expected_name);

    auto reconstructed_property = dynamic_cast<mvvm::PropertyItem*>(reconstructed_item.get());
    ASSERT_NE(reconstructed_property, nullptr);
    EXPECT_EQ(reconstructed_property->Data<int>(), 42);
  }
}

TEST_F(MimeConverionHelperTests, CreateTwoItemsFromMime)
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

  auto reconstructed_items = CreateSessionItems(data.get(), mime_type);
  ASSERT_EQ(reconstructed_items.size(), 2);

  auto reconstructed_property = reconstructed_items.at(0).get();
  auto reconstructed_compound = reconstructed_items.at(1).get();

  EXPECT_EQ(reconstructed_property->GetDisplayName(), expected_name1);
  EXPECT_EQ(reconstructed_property->GetType(), mvvm::PropertyItem::GetStaticType());
  EXPECT_EQ(reconstructed_property->Data<int>(), 42);
  EXPECT_EQ(reconstructed_compound->GetDisplayName(), expected_name2);
  EXPECT_EQ(reconstructed_compound->GetType(), mvvm::CompoundItem::GetStaticType());
}

TEST_F(MimeConverionHelperTests, CoopyAndPasteWithFiltering)
{
  const QString mime_type = "application.coa.tests";
  const std::string expected_name1("abc");
  const std::string expected_name2("def");

  mvvm::CompoundItem compound;
  compound.SetDisplayName(expected_name1);
  compound.RegisterTag(mvvm::TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child0 =
      compound.InsertItem(std::make_unique<mvvm::PropertyItem>(), mvvm::TagIndex::Append());
  child0->SetDisplayName("child_name0");
  auto child1 =
      compound.InsertItem(std::make_unique<mvvm::PropertyItem>(), mvvm::TagIndex::Append());
  child1->SetDisplayName("child_name1");

  auto filter_func = [](const auto& item) -> bool
  { return item.GetDisplayName() == "child_name0"; };

  auto data = CreateCopyMimeData(compound, mime_type, filter_func);

  auto reconstructed_items = CreateSessionItems(data.get(), mime_type);
  ASSERT_EQ(reconstructed_items.size(), 1);

  auto reconstructed_compound = reconstructed_items.at(0).get();
  EXPECT_EQ(reconstructed_compound->GetItemCount("defaultTag"), 1);
  EXPECT_EQ(reconstructed_compound->GetItem("defaultTag")->GetDisplayName(),
            std::string("child_name1"));
}
