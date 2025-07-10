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

#include "sup/gui/mainwindow/settings_helper.h"

#include <sup/gui/model/settings_model.h>

#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/viewmodel/variant_converter.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for methods from settings_helper.h
 */
class SettingsHelperTest : public ::testing::Test
{
public:
  ::testing::MockFunction<read_variant_func_t> m_mock_read_func;
  ::testing::MockFunction<write_variant_func_t> m_mock_write_func;

  class TestModel : public mvvm::SessionModel
  {
  public:
    TestModel() : SessionModel("TestModel") {}
  };
};

TEST_F(SettingsHelperTest, WriteAndReadEmptyModel)
{
  mvvm::SessionModel model;

  EXPECT_CALL(m_mock_write_func, Call(::testing::_, ::testing::_)).Times(0);
  WriteApplicationSettings(model, m_mock_write_func.AsStdFunction());

  EXPECT_CALL(m_mock_read_func, Call(::testing::_)).Times(0);
  ReadSettingsFromPersistentStorage(model, m_mock_read_func.AsStdFunction());
}

TEST_F(SettingsHelperTest, WriteModelWithSinglePropertyItem)
{
  const std::string display_name("display_name");
  TestModel model;
  auto item = model.InsertItem<mvvm::PropertyItem>();
  item->SetDisplayName(display_name);
  item->SetData(mvvm::int32{42});

  const auto expected_variant = mvvm::GetQtVariant(item->Data());
  const QString expected_key = "TestModel/display_name";
  EXPECT_CALL(m_mock_write_func, Call(expected_key, expected_variant)).Times(1);
  WriteApplicationSettings(model, m_mock_write_func.AsStdFunction());
}

TEST_F(SettingsHelperTest, WriteModelWithTwoPropertyItems)
{
  TestModel model;
  auto item0 = model.InsertItem<mvvm::PropertyItem>();
  item0->SetDisplayName("Number");
  item0->SetData(mvvm::int32{42});

  auto item1 = model.InsertItem<mvvm::PropertyItem>();
  item1->SetDisplayName("Name");
  item1->SetData(std::string("abc"));

  EXPECT_CALL(m_mock_write_func, Call(QString("TestModel/Number"), QVariant(42))).Times(1);
  EXPECT_CALL(m_mock_write_func,
              Call(QString("TestModel/Name"), QVariant::fromValue(QString("abc"))))
      .Times(1);
  WriteApplicationSettings(model, m_mock_write_func.AsStdFunction());
}

TEST_F(SettingsHelperTest, WriteModelWithPropertyItemCompound)
{
  TestModel model;
  auto parent = model.InsertItem<mvvm::CompoundItem>();
  parent->RegisterTag(mvvm::TagInfo::CreateUniversalTag("Tag"), true);
  parent->SetDisplayName("Compound");
  auto item0 = model.InsertItem<mvvm::PropertyItem>(parent, mvvm::TagIndex::Append());
  item0->SetDisplayName("Number");
  item0->SetData(mvvm::int32{42});

  auto item1 = model.InsertItem<mvvm::PropertyItem>(parent, mvvm::TagIndex::Append());
  item1->SetDisplayName("Name");
  item1->SetData(std::string("abc"));

  EXPECT_CALL(m_mock_write_func, Call(QString("TestModel/Compound/Number"), QVariant(42))).Times(1);
  EXPECT_CALL(m_mock_write_func,
              Call(QString("TestModel/Compound/Name"), QVariant::fromValue(QString("abc"))))
      .Times(1);
  WriteApplicationSettings(model, m_mock_write_func.AsStdFunction());
}

TEST_F(SettingsHelperTest, ReadModelFromEmptyConfig)
{
  const std::string display_name("display_name");
  TestModel model;
  auto item = model.InsertItem<mvvm::PropertyItem>();
  item->SetDisplayName(display_name);
  item->SetData(mvvm::int32{42});

  // instructing to return an invalid variant as a sign that setting doesn't exist
  const auto expected_variant = QVariant();
  const QString key = "TestModel/display_name";
  ON_CALL(m_mock_read_func, Call(key)).WillByDefault(::testing::Return(expected_variant));

  EXPECT_CALL(m_mock_read_func, Call(key)).Times(1);

  EXPECT_NO_THROW(ReadSettingsFromPersistentStorage(model, m_mock_read_func.AsStdFunction()));

  // item data remains the same
  EXPECT_EQ(item->Data<mvvm::int32>(), 42);
}

TEST_F(SettingsHelperTest, ReadModelWithSinglePropertyItem)
{
  const std::string display_name("display_name");
  TestModel model;
  auto item = model.InsertItem<mvvm::PropertyItem>();
  item->SetDisplayName(display_name);
  item->SetData(mvvm::int32{42});

  // preparing another value
  const auto expected_variant = QVariant::fromValue(43);
  const QString key = "TestModel/display_name";
  ON_CALL(m_mock_read_func, Call(key)).WillByDefault(::testing::Return(expected_variant));

  EXPECT_CALL(m_mock_read_func, Call(key)).Times(1);
  ReadSettingsFromPersistentStorage(model, m_mock_read_func.AsStdFunction());

  // checking that item data was updated
  EXPECT_EQ(item->Data<mvvm::int32>(), 43);
}

TEST_F(SettingsHelperTest, ReadModelWithTwoPropertyItems)
{
  TestModel model;
  auto item0 = model.InsertItem<mvvm::PropertyItem>();
  item0->SetDisplayName("Number");
  item0->SetData(mvvm::int32{42});

  auto item1 = model.InsertItem<mvvm::PropertyItem>();
  item1->SetDisplayName("Name");
  item1->SetData(std::string("abc"));

  // preparing new values
  const QString key1("TestModel/Number");
  const QString key2("TestModel/Name");

  ON_CALL(m_mock_read_func, Call(key1)).WillByDefault(::testing::Return(QVariant(43)));
  ON_CALL(m_mock_read_func, Call(key2))
      .WillByDefault(::testing::Return(QVariant::fromValue(QString("def"))));

  EXPECT_CALL(m_mock_read_func, Call(key1)).Times(1);
  EXPECT_CALL(m_mock_read_func, Call(key2)).Times(1);
  ReadSettingsFromPersistentStorage(model, m_mock_read_func.AsStdFunction());

  // checking that item data was updated
  EXPECT_EQ(item0->Data<mvvm::int32>(), 43);
  EXPECT_EQ(item1->Data<std::string>(), std::string("def"));
}

TEST_F(SettingsHelperTest, ReadModelWithPropertyItemCompound)
{
  TestModel model;
  auto parent = model.InsertItem<mvvm::CompoundItem>();
  parent->RegisterTag(mvvm::TagInfo::CreateUniversalTag("Tag"), true);
  parent->SetDisplayName("Compound");
  auto item0 = model.InsertItem<mvvm::PropertyItem>(parent, mvvm::TagIndex::Append());
  item0->SetDisplayName("Number");
  item0->SetData(mvvm::int32{42});

  auto item1 = model.InsertItem<mvvm::PropertyItem>(parent, mvvm::TagIndex::Append());
  item1->SetDisplayName("Name");
  item1->SetData(std::string("abc"));

  // preparing new values
  const QString key1("TestModel/Compound/Number");
  const QString key2("TestModel/Compound/Name");

  ON_CALL(m_mock_read_func, Call(key1)).WillByDefault(::testing::Return(QVariant(43)));
  ON_CALL(m_mock_read_func, Call(key2))
      .WillByDefault(::testing::Return(QVariant::fromValue(QString("def"))));

  EXPECT_CALL(m_mock_read_func, Call(key1)).Times(1);
  EXPECT_CALL(m_mock_read_func, Call(key2)).Times(1);
  ReadSettingsFromPersistentStorage(model, m_mock_read_func.AsStdFunction());

  // checking that item data was updated
  EXPECT_EQ(item0->Data<mvvm::int32>(), 43);
  EXPECT_EQ(item1->Data<std::string>(), std::string("def"));
}

}  // namespace sup::gui::test
