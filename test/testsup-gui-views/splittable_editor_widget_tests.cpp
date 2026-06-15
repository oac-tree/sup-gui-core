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

#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>
#include <sup/gui/views/dtoeditor/splittable_editor_controller.h>

#include <mvvm/model/item_selection.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QSplitter>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(mvvm::ItemSelection)
#endif

namespace sup::gui::test
{

/**
 * @brief Tests for SplittableEditorWidget class.
 */
class SplittableEditorWidgetTest : public ::testing::Test
{
public:
};

TEST_F(SplittableEditorWidgetTest, InitialState)
{
  SplittableEditorWidget editor;
  editor.show();

  ASSERT_NE(editor.GetController(), nullptr);

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

TEST_F(SplittableEditorWidgetTest, SetModelAfterPanelCreation)
{
  ConfigurationModel model;

  SplittableEditorWidget editor;
  editor.show();

  editor.CreatePanel();
  editor.SetModel(&model);
  editor.CreatePanel();

  auto panels = editor.GetController()->GetPanels();
  ASSERT_EQ(panels.size(), 2);
  EXPECT_EQ(panels.at(0)->GetModel(), &model);
  EXPECT_EQ(panels.at(1)->GetModel(), &model);
}

TEST_F(SplittableEditorWidgetTest, focusPanelSelectionChanged)
{
  const ConfigurationItem item;
  const mvvm::ItemSelection selection(&item);

  SplittableEditorWidget editor;
  editor.show();

  editor.CreatePanel();

  QSignalSpy spy_selection_changed(&editor, &SplittableEditorWidget::focusPanelSelectionChanged);

  // emit selection changed from controller
  editor.GetController()->focusPanelSelectionChanged(selection);
  ASSERT_EQ(spy_selection_changed.count(), 1);
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::ItemSelection>(spy_selection_changed), selection);
}

}  // namespace sup::gui::test