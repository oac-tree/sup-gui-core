/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sup/gui/components/dto_editor_project.h"

#include <sup/gui/app/app_constants.h>
#include <sup/gui/model/sup_dto_model.h>
#include <sup/gui/model/waveform_model.h>

#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/utils/file_utils.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/folder_test.h>

using namespace sup::gui;

/**
 * @brief Tests for DtoEditorProject class.
 */
class DtoEditorProjectTest : public testutils::FolderTest
{
public:
  DtoEditorProjectTest() : FolderTest("test_DtoEditorProjectTest") {}

  std::unique_ptr<DtoEditorProject> CreateProject()
  {
    return std::make_unique<DtoEditorProject>(m_modified_callback.AsStdFunction(),
                                              m_loaded_callback.AsStdFunction());
  }

  ::testing::MockFunction<void(void)> m_modified_callback;
  ::testing::MockFunction<void(void)> m_loaded_callback;
};

TEST_F(DtoEditorProjectTest, InitialState)
{
  auto project = CreateProject();

  EXPECT_TRUE(project->GetPath().empty());
  EXPECT_EQ(project->GetProjectType(), mvvm::ProjectType::kFileBased);
  EXPECT_EQ(project->GetApplicationType(), constants::kDtoEditorApplicationType.toStdString());

  EXPECT_EQ(project->GetSupDtoModel(), nullptr);
  EXPECT_EQ(project->GetWaveformModel(), nullptr);
  EXPECT_FALSE(project->IsModified());
}

TEST_F(DtoEditorProjectTest, CreateNewProjectThenModifyThenClose)
{
  auto project = CreateProject();

  // setting up expectations before project creation
  EXPECT_CALL(m_loaded_callback, Call()).Times(1);

  EXPECT_TRUE(project->CreateEmpty());

  EXPECT_FALSE(project->HasPath());
  EXPECT_NE(project->GetSupDtoModel(), nullptr);
  EXPECT_NE(project->GetWaveformModel(), nullptr);
  EXPECT_FALSE(project->IsModified());

  // setting up expectation before project modification
  EXPECT_CALL(m_modified_callback, Call()).Times(1);

  // modifying a project
  project->GetWaveformModel()->InsertItem<mvvm::SessionItem>();
  project->GetSupDtoModel()->InsertItem<mvvm::SessionItem>();
  EXPECT_TRUE(project->IsModified());

  // closing project
  EXPECT_TRUE(project->Close());
  EXPECT_EQ(project->GetWaveformModel(), nullptr);
  EXPECT_EQ(project->GetSupDtoModel(), nullptr);
  EXPECT_FALSE(project->IsModified());
}
