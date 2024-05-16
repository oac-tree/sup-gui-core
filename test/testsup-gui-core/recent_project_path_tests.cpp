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

#include "sup/gui/components/recent_project_path.h"

#include <gtest/gtest.h>
#include <testutils/folder_based_test.h>
#include <testutils/test_utils.h>

using namespace sup::gui;

/**
 * @brief The RecentProjectPathTest class tests RecentProjectPath.
 */
class RecentProjectPathTest : public testutils::FolderBasedTest
{
public:
  RecentProjectPathTest() : testutils::FolderBasedTest("test_RecentProjectPath") {}
};

TEST_F(RecentProjectPathTest, InitialState)
{
  const int kMaxProjectCount{0};
  RecentProjectPath projects(kMaxProjectCount);
  EXPECT_TRUE(projects.GetRecentProjectList().empty());
  EXPECT_TRUE(projects.GetCurrentWorkdir().isEmpty());
}

TEST_F(RecentProjectPathTest, SetCurrentWorkdir)
{
  const int kMaxProjectCount{0};
  RecentProjectPath projects(kMaxProjectCount);

  projects.SetCurrentWorkdir("abc");
  EXPECT_EQ(projects.GetCurrentWorkdir(), QString("abc"));

  projects.SetRecentProjectList(QStringList({"a1", "a2"}));

  // it doesn't keep recent projects list, since
  EXPECT_TRUE(projects.GetRecentProjectList().isEmpty());
}

TEST_F(RecentProjectPathTest, AddToRecentProjectList)
{
  const int kMaxProjectCount{3};
  RecentProjectPath projects(kMaxProjectCount);

  // creating text files in "<build>/test_output/test_RecentProjectPath"
  auto path1 = GetFilePath("a1.txt");
  auto path2 = GetFilePath("a2.txt");
  auto path3 = GetFilePath("a3.txt");
  auto path4 = GetFilePath("a4.txt");
  testutils::CreateTextFile(path1, "");
  testutils::CreateTextFile(path2, "");
  testutils::CreateTextFile(path3, "");
  testutils::CreateTextFile(path4, "");

  // updating recent project paths
  projects.AddToRecentProjectList(QString::fromStdString(path1));
  projects.AddToRecentProjectList(QString::fromStdString(path2));

  QStringList expected_paths({QString::fromStdString(path2), QString::fromStdString(path1)});
  EXPECT_EQ(projects.GetRecentProjectList(), expected_paths);

  // Adding again path1. The order should change, no duplications.
  projects.AddToRecentProjectList(QString::fromStdString(path1));
  expected_paths = QStringList({QString::fromStdString(path1), QString::fromStdString(path2)});
  EXPECT_EQ(projects.GetRecentProjectList(), expected_paths);

  // Adding two more paths. Since the maximum allowed number of path is 3, path2 should disappear
  // from the list.
  projects.AddToRecentProjectList(QString::fromStdString(path3));
  projects.AddToRecentProjectList(QString::fromStdString(path4));
  expected_paths = QStringList({QString::fromStdString(path4), QString::fromStdString(path3),
                                QString::fromStdString(path1)});
}

TEST_F(RecentProjectPathTest, UpdateCurrentWorkdir)
{
  const int kMaxProjectCount{3};
  RecentProjectPath projects(kMaxProjectCount);

  // creating text files in "<build>/test_output/test_RecentProjectPath"
  auto path = GetFilePath("a5.txt");

  // working dir for file is its parent directory
  projects.UpdateCurrentWorkdir(QString::fromStdString(path));
  EXPECT_EQ(projects.GetCurrentWorkdir(), QString::fromStdString(GetTestHomeDir()));

  // working dir for dir is its   parent directory
  projects.UpdateCurrentWorkdir(QString::fromStdString(GetTestHomeDir()));
  EXPECT_EQ(projects.GetCurrentWorkdir(),
            QString::fromStdString(testutils::GetTestSuiteOutputDir()));
}
