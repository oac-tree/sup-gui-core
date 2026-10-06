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

#include "sup/gui/settings/project_settings_storage.h"

#include <sup/gui/model/settings_container_item.h>
#include <sup/gui/settings/settings_archive.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/serialization/xml_document.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

namespace sup::gui::test
{

namespace
{

constexpr auto kValueKey = "Group/name";

struct TestInfo
{
  std::string state;
  std::vector<std::string> names;
};

bool operator==(const TestInfo& lhs, const TestInfo& rhs)
{
  return lhs.state == rhs.state && lhs.names == rhs.names;
}

template <typename Archive>
void Visit(Archive& archive, TestInfo& info)
{
  archive("state", info.state);
  archive("names", info.names);
}

}  // namespace

/**
 * @brief Tests for ProjectSettingsStorage class.
 */
class ProjectSettingsStorageTest : public test::FolderTest
{
public:
  ProjectSettingsStorageTest() : FolderTest("ProjectSettingsStorageTest") {}

  //! Returns storage working with the model of the fixture.
  ProjectSettingsStorage CreateStorage()
  {
    return ProjectSettingsStorage([this]() { return m_model; });
  }

  //! Returns the number of settings containers in the model.
  static std::size_t GetContainerCount(mvvm::ApplicationModel& model)
  {
    return mvvm::utils::GetTopItems<SettingsContainerItem>(model).size();
  }

  mvvm::ApplicationModel* m_model{nullptr};
};

TEST_F(ProjectSettingsStorageTest, NoModel)
{
  auto storage = CreateStorage();

  EXPECT_FALSE(storage.Read(kValueKey).has_value());
  EXPECT_NO_THROW(storage.Write(kValueKey, std::string("abc")));
}

TEST_F(ProjectSettingsStorageTest, ReadDoesNotCreateContainer)
{
  mvvm::ApplicationModel model;
  m_model = &model;
  auto storage = CreateStorage();

  EXPECT_FALSE(storage.Read(kValueKey).has_value());
  EXPECT_EQ(GetContainerCount(model), 0);
}

TEST_F(ProjectSettingsStorageTest, WriteAndRead)
{
  mvvm::ApplicationModel model;
  m_model = &model;
  auto storage = CreateStorage();

  storage.Write(kValueKey, std::string("abc"));
  storage.Write("Group/size", mvvm::int32{2});

  EXPECT_EQ(GetContainerCount(model), 1);
  EXPECT_EQ(storage.Read(kValueKey), mvvm::variant_t{std::string("abc")});
  EXPECT_EQ(storage.Read("Group/size"), mvvm::variant_t{mvvm::int32{2}});
  EXPECT_FALSE(storage.Read("Group/other").has_value());
}

TEST_F(ProjectSettingsStorageTest, FollowsCurrentModel)
{
  mvvm::ApplicationModel model0;
  mvvm::ApplicationModel model1;
  auto storage = CreateStorage();

  m_model = &model0;
  storage.Write(kValueKey, std::string("abc"));

  m_model = &model1;
  EXPECT_FALSE(storage.Read(kValueKey).has_value());
  storage.Write(kValueKey, std::string("def"));

  m_model = &model0;
  EXPECT_EQ(storage.Read(kValueKey), mvvm::variant_t{std::string("abc")});

  m_model = nullptr;
  EXPECT_FALSE(storage.Read(kValueKey).has_value());
}

TEST_F(ProjectSettingsStorageTest, XmlDocumentSaveLoad)
{
  const auto file_path = GetFilePath("XmlDocumentSaveLoad.xml");

  mvvm::ApplicationModel model;
  m_model = &model;
  auto storage = CreateStorage();

  const TestInfo info{"state", {"proc0", "proc1"}};
  WriteToStorage(storage, kValueKey, info);

  mvvm::XmlDocument document({&model});
  document.Save(file_path);
  model.Clear();
  ASSERT_EQ(GetContainerCount(model), 0);

  document.Load(file_path);

  EXPECT_EQ(ReadFromStorage<TestInfo>(storage, kValueKey), info);
}

}  // namespace sup::gui::test
