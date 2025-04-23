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

#include "sup/gui/components/anyvalue_editor_project.h"

#include <sup/gui/app/app_constants.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/test/mock_project_context.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/utils/file_utils.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/folder_test.h>

using namespace sup::gui;

/**
 * @brief Tests for AnyValueEditorProject class.
 */
class AnyValueEditorProjectTest : public test::FolderTest
{
public:
  AnyValueEditorProjectTest() : FolderTest("AnyValueEditorProjectTest") {}

  std::unique_ptr<AnyValueEditorProject> CreateProject()
  {
    return std::make_unique<AnyValueEditorProject>(m_mock_project_context.CreateContext(
        constants::kAnyValueEditorApplicationType.toStdString()));
  }

  mvvm::test::MockProjectContext m_mock_project_context;
};

TEST_F(AnyValueEditorProjectTest, InitialState)
{
  auto project = CreateProject();

  EXPECT_FALSE(project->HasPath());
  EXPECT_EQ(project->GetProjectType(), mvvm::ProjectType::kFileBased);
  EXPECT_EQ(project->GetApplicationType(), constants::kAnyValueEditorApplicationType.toStdString());

  EXPECT_EQ(project->GetApplicationModel(), nullptr);
  EXPECT_TRUE(project->GetModels().empty());
  EXPECT_FALSE(project->IsModified());
}

TEST_F(AnyValueEditorProjectTest, CreateNewProjectThenModifyThenClose)
{
  auto project = CreateProject();

  // setting up expectations before project creation
  EXPECT_CALL(m_mock_project_context, OnLoaded()).Times(1);

  EXPECT_TRUE(project->CreateEmpty());

  EXPECT_FALSE(project->HasPath());
  EXPECT_NE(project->GetApplicationModel(), nullptr);
  ASSERT_FALSE(project->GetModels().empty());
  EXPECT_NE(project->GetModels().at(0), nullptr);
  EXPECT_FALSE(project->IsModified());

  // setting up expectation before project modification
  EXPECT_CALL(m_mock_project_context, OnModified()).Times(1);

  // modifying a project
  project->GetApplicationModel()->InsertItem<mvvm::SessionItem>();
  EXPECT_TRUE(project->IsModified());

  // closing project
  EXPECT_TRUE(project->Close());
  EXPECT_EQ(project->GetApplicationModel(), nullptr);
  EXPECT_FALSE(project->IsModified());
}

TEST_F(AnyValueEditorProjectTest, SaveAndClose)
{
  const auto expected_path = GetFilePath("untitled.xml");

  auto project = CreateProject();

  // setting up expectations before project creation
  EXPECT_CALL(m_mock_project_context, OnLoaded()).Times(1);

  EXPECT_TRUE(project->CreateEmpty());

  // setting up expectation before project modification
  EXPECT_CALL(m_mock_project_context, OnModified()).Times(1);

  project->GetApplicationModel()->InsertItem<mvvm::SessionItem>();
  EXPECT_TRUE(project->IsModified());

  auto previous_model = project->GetApplicationModel();

  EXPECT_CALL(m_mock_project_context, OnSaved()).Times(1);
  EXPECT_TRUE(project->Save(expected_path));

  EXPECT_EQ(project->GetPath(), expected_path);
  EXPECT_EQ(project->GetApplicationModel(), previous_model);
  EXPECT_FALSE(project->IsModified());

  // closing project
  EXPECT_TRUE(project->Close());
  EXPECT_EQ(project->GetApplicationModel(), nullptr);
  EXPECT_FALSE(project->IsModified());

  EXPECT_TRUE(mvvm::utils::IsExists(expected_path));
}

TEST_F(AnyValueEditorProjectTest, SaveAndLoad)
{
  const auto expected_path = GetFilePath("untitled2.xml");

  auto project = CreateProject();

  // setting up expectations before project creation
  EXPECT_CALL(m_mock_project_context, OnLoaded()).Times(1);

  EXPECT_TRUE(project->CreateEmpty());

  // setting up expectation before project modification
  EXPECT_CALL(m_mock_project_context, OnModified()).Times(1);

  auto item = project->GetApplicationModel()->InsertItem<mvvm::SessionItem>();
  item->SetData(42);
  EXPECT_TRUE(project->IsModified());

  EXPECT_CALL(m_mock_project_context, OnSaved()).Times(1);
  EXPECT_TRUE(project->Save(expected_path));

  // setting up expectation before project modification
  EXPECT_CALL(m_mock_project_context, OnModified()).Times(1);

  // modifying further after save was made
  item->SetData(43);

  // setting up expectation before document load
  EXPECT_CALL(m_mock_project_context, OnLoaded()).Times(1);

  EXPECT_TRUE(project->Load(expected_path));
  EXPECT_FALSE(project->IsModified());

  // checking content of the model
  auto recreated_model = project->GetApplicationModel();
  ASSERT_NE(recreated_model, nullptr);
  ASSERT_NE(recreated_model->GetRootItem()->GetItem(mvvm::TagIndex::First()), nullptr);
  // expecting old value which we had at the moment of save
  EXPECT_EQ(recreated_model->GetRootItem()->GetItem(mvvm::TagIndex::First())->Data<int>(), 42);
}
