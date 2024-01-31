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

#include "sup/gui/dtoeditor/dto_composer_tab_controller.h"

#include <sup/gui/anyvalueeditor/anyvalue_editor_widget.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QTabWidget>

using namespace sup::gui;

class DtoComposerTabControllerTest : public ::testing::Test
{
public:
  mvvm::ApplicationModel m_model;
};

TEST_F(DtoComposerTabControllerTest, InitialState)
{
  QTabWidget tab_widget;
  DtoComposerTabController controller(&m_model, &tab_widget);

  EXPECT_EQ(tab_widget.count(), 0);

  mvvm::SessionItem item;
  EXPECT_EQ(controller.GetWidgetForItem(&item), nullptr);
}

//! Testing the case, when the model contains AnyValueItem container from a start. There must be one
//! tab created after the initialization.
TEST_F(DtoComposerTabControllerTest, ModelWithSingleContainer)
{
  auto container = m_model.InsertItem<mvvm::ContainerItem>();

  QTabWidget tab_widget;
  DtoComposerTabController controller(&m_model, &tab_widget);

  EXPECT_EQ(tab_widget.count(), 1);
  EXPECT_EQ(controller.GetWidgetForItem(container), tab_widget.widget(0));
}
