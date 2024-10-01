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

#include "sup/gui/views/dtoeditor/dto_composer_view.h"

#include <sup/gui/model/sup_dto_model.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_widget.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

#include <QTabWidget>

using namespace sup::gui;

/**
 * @brief Tests for DtoComposerView class.
 */
class DtoComposerViewTest : public ::testing::Test
{
public:
  mvvm::ApplicationModel m_model;
};

//! This mimick loading of the project from disk.
TEST_F(DtoComposerViewTest, ReplaceRootItem)
{
  SupDtoModel model;  // has one container by default

  DtoComposerView view(&model);

  auto tab_widget = view.findChild<QTabWidget*>();
  ASSERT_NE(tab_widget, nullptr);

  EXPECT_EQ(tab_widget->count(), 1);

  auto new_root = mvvm::utils::CreateEmptyRootItem();
  new_root->InsertItem<mvvm::ContainerItem>(mvvm::TagIndex::Append());
  new_root->InsertItem<mvvm::ContainerItem>(mvvm::TagIndex::Append());

  model.ReplaceRootItem(std::move(new_root));
  EXPECT_EQ(tab_widget->count(), 2);
}
