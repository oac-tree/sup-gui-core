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

#include "sup/gui/views/dtoeditor/dto_compare_view.h"

#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>
#include <sup/gui/views/dtoeditor/dto_explorer_panel.h>
#include <sup/gui/views/dtoeditor/splittable_editor_widget.h>

#include <gtest/gtest.h>

namespace sup::gui::test
{

/**
 * @brief Tests for DtoCompareViewTest class.
 */
class DtoCompareViewTest : public ::testing::Test
{
};

TEST_F(DtoCompareViewTest, SelectionLogic)
{
  ConfigurationModel model;
  auto configuration0 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration0->SetDisplayName("Config0");
  auto configuration1 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                            mvvm::TagIndex::Append());
  configuration1->SetDisplayName("Config1");

  DtoCompareView view;
  view.show();

  auto explorer_panel = view.findChild<DtoExplorerPanel*>();
  ASSERT_NE(explorer_panel, nullptr);
  auto splittable_config_editor = view.findChild<SplittableEditorWidget*>();

  view.SetConfigurationModel(&model);
}

}  // namespace sup::gui::test
