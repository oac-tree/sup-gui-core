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

#include "sup/gui/views/dtoeditor/configuration_list_widget.h"

#include <sup/gui/model/configuration_container_item.h>
#include <sup/gui/model/configuration_item.h>
#include <sup/gui/model/configuration_model.h>

#include <mvvm/model/item_selection.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(mvvm::ItemSelection)
#endif

namespace sup::gui::test
{

/**
 * @brief Tests for ConfigurationListWidget class.
 */
class ConfigurationListWidgetTest : public ::testing::Test
{
};

TEST_F(ConfigurationListWidgetTest, InitialState)
{
  const ConfigurationListWidget widget;

  EXPECT_EQ(widget.GetSelection(), mvvm::ItemSelection());
}

TEST_F(ConfigurationListWidgetTest, SelectConfiguration)
{
  ConfigurationModel model;
  auto config0 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                     mvvm::TagIndex::Append());
  auto config1 = model.InsertItem<ConfigurationItem>(model.GetConfigurationContainer(),
                                                     mvvm::TagIndex::Append());
  ConfigurationListWidget widget;
  widget.SetConfigurationContainer(model.GetConfigurationContainer());

  EXPECT_TRUE(widget.GetSelection().IsEmpty());
  QSignalSpy selection_changed_spy(&widget, &ConfigurationListWidget::SelectionChanged);

  widget.SetSelection(mvvm::ItemSelection(config0));

  EXPECT_EQ(mvvm::test::TakeValue<mvvm::ItemSelection>(selection_changed_spy),
            mvvm::ItemSelection(config0));

  EXPECT_EQ(widget.GetSelection(), mvvm::ItemSelection(config0));

  // selecting the same second time
  widget.SetSelection(mvvm::ItemSelection(config0));
  EXPECT_EQ(widget.GetSelection(), mvvm::ItemSelection(config0));

  EXPECT_EQ(selection_changed_spy.count(), 0);  // was consumed by the first GetSendItem

  // removing selection
  widget.SetSelection(mvvm::ItemSelection());
  EXPECT_EQ(mvvm::test::TakeValue<mvvm::ItemSelection>(selection_changed_spy),
            mvvm::ItemSelection(nullptr));
  EXPECT_TRUE(widget.GetSelection().IsEmpty());
}

}  // namespace sup::gui::test
