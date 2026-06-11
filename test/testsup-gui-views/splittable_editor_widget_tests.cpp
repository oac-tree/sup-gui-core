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

#include "sup/gui/views/dtoeditor/splittable_editor_widget.h"

#include <gtest/gtest.h>

#include <QSplitter>

namespace sup::gui::test
{

/**
 * @brief Tests for SplittableEditorWidget class.
 */
class SplittableEditorWidgetTest : public ::testing::Test
{
};

TEST_F(SplittableEditorWidgetTest, InitialState)
{
  SplittableEditorWidget editor;
  editor.show();

  auto splitter = editor.findChild<QSplitter*>();

  // empty splitter
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 0);
}

TEST_F(SplittableEditorWidgetTest, CreatePanel)
{
  SplittableEditorWidget editor;
  editor.show();

  auto splitter = editor.findChild<QSplitter*>();

  // add first panel
  editor.CreatePanel();

  ASSERT_EQ(splitter->count(), 1);
}

}  // namespace sup::gui::test