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

#include "sup/gui/views/dtoeditor/dto_composer_tab_controller.h"

#include <sup/gui/views/anyvalueeditor/anyvalue_editor_widget.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

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

//! Inserting container in a model after initialization. New tab should be created.
TEST_F(DtoComposerTabControllerTest, InsertContainer)
{
  QTabWidget tab_widget;
  DtoComposerTabController controller(&m_model, &tab_widget);

  // inserting after initialization
  auto container = m_model.InsertItem<mvvm::ContainerItem>();

  EXPECT_EQ(tab_widget.count(), 1);
  EXPECT_EQ(controller.GetWidgetForItem(container), tab_widget.widget(0));
}

TEST_F(DtoComposerTabControllerTest, RemoveContainer)
{
  auto container = m_model.InsertItem<mvvm::ContainerItem>();

  QTabWidget tab_widget;
  DtoComposerTabController controller(&m_model, &tab_widget);

  m_model.RemoveItem(container);

  EXPECT_EQ(tab_widget.count(), 0);
  EXPECT_EQ(controller.GetWidgetForItem(container), nullptr);
}

//! Inserting container between two existing containers.
TEST_F(DtoComposerTabControllerTest, InsertContainerBetweenAndThenRemove)
{
  auto container0 =
      m_model.InsertItem<mvvm::ContainerItem>(m_model.GetRootItem(), mvvm::TagIndex::Default(0));
  auto container1 =
      m_model.InsertItem<mvvm::ContainerItem>(m_model.GetRootItem(), mvvm::TagIndex::Default(1));

  QTabWidget tab_widget;
  DtoComposerTabController controller(&m_model, &tab_widget);

  // inserting after initialization between container0 and container 1
  auto container2 =
      m_model.InsertItem<mvvm::ContainerItem>(m_model.GetRootItem(), mvvm::TagIndex::Default(1));
  EXPECT_EQ(m_model.GetRootItem()->GetAllItems(),
            std::vector<mvvm::SessionItem*>({container0, container2, container1}));

  EXPECT_EQ(tab_widget.count(), 3);
  EXPECT_EQ(controller.GetWidgetForItem(container0), tab_widget.widget(0));
  EXPECT_EQ(controller.GetWidgetForItem(container2), tab_widget.widget(1));
  EXPECT_EQ(controller.GetWidgetForItem(container1), tab_widget.widget(2));

  // removing middle container
  m_model.RemoveItem(container2);
  EXPECT_EQ(tab_widget.count(), 2);
  EXPECT_EQ(controller.GetWidgetForItem(container0), tab_widget.widget(0));
  EXPECT_EQ(controller.GetWidgetForItem(container1), tab_widget.widget(1));
}
