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

#include "sup/gui/views/dtoeditor/splittable_editor_controller.h"

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <gtest/gtest.h>

#include <QSplitter>

namespace sup::gui::test
{

class SplittableEditorControllerTest : public ::testing::Test
{
public:
  QSplitter m_splitter;
};

TEST_F(SplittableEditorControllerTest, InitialState)
{
  EXPECT_THROW(const SplittableEditorController controller(nullptr), RuntimeException);

  SplittableEditorController controller(&m_splitter);
  EXPECT_EQ(controller.GetFocusWidget(), nullptr);
}

}  // namespace sup::gui::test
