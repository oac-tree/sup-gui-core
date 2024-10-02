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

#include <sup/gui/components/dto_composer_tab_controller.h>

#include <sup/gui/model/sup_dto_model.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

#include <QTabWidget>

using namespace sup::gui;

/**
 * @brief Tests for DtoComposerTabController class.
 */
class DtoComposerTabControllerTest : public ::testing::Test
{
public:
  mvvm::ApplicationModel m_model;

  static DtoComposerTabController::create_widget_callback_t CreateCallback()
  {
    return [](mvvm::SessionItem* item)
    {
      (void)item;
      return std::make_unique<QWidget>();
    };
  }
};

TEST_F(DtoComposerTabControllerTest, InitialState)
{
  QTabWidget tab_widget;
  DtoComposerTabController controller(&m_model, CreateCallback(), &tab_widget);

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
  DtoComposerTabController controller(&m_model, CreateCallback(), &tab_widget);

  EXPECT_EQ(tab_widget.count(), 1);
  EXPECT_EQ(controller.GetWidgetForItem(container), tab_widget.widget(0));
}

//! Inserting container in a model after initialization. New tab should be created.
TEST_F(DtoComposerTabControllerTest, InsertContainer)
{
  QTabWidget tab_widget;
  DtoComposerTabController controller(&m_model, CreateCallback(), &tab_widget);

  // inserting after initialization
  auto container = m_model.InsertItem<mvvm::ContainerItem>();

  EXPECT_EQ(tab_widget.count(), 1);
  EXPECT_EQ(controller.GetWidgetForItem(container), tab_widget.widget(0));
}

TEST_F(DtoComposerTabControllerTest, RemoveContainer)
{
  auto container = m_model.InsertItem<mvvm::ContainerItem>();

  QTabWidget tab_widget;
  DtoComposerTabController controller(&m_model, CreateCallback(), &tab_widget);

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
  DtoComposerTabController controller(&m_model, CreateCallback(), &tab_widget);

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

//! Controller should cleanup tabs after model reset.
TEST_F(DtoComposerTabControllerTest, ResetApplicationModel)
{
  auto container = m_model.InsertItem<mvvm::ContainerItem>();

  QTabWidget tab_widget;
  const DtoComposerTabController controller(&m_model, CreateCallback(), &tab_widget);

  EXPECT_EQ(tab_widget.count(), 1);

  m_model.Clear();
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  EXPECT_EQ(tab_widget.count(), 0);
}

//! Controller should recreate default tab after SupDtoModel reset.
TEST_F(DtoComposerTabControllerTest, ResetSupDtoModel)
{
  SupDtoModel model;  // has one container by default

  QTabWidget tab_widget;
  const DtoComposerTabController controller(&model, CreateCallback(), &tab_widget);

  EXPECT_EQ(tab_widget.count(), 1);

  model.Clear();
  EXPECT_EQ(model.GetRootItem()->GetTotalItemCount(), 1);

  // tab was recreated because model has recreated default container
  EXPECT_EQ(tab_widget.count(), 1);
}

//! This mimick loading of the project from disk.
TEST_F(DtoComposerTabControllerTest, ReplaceRootItem)
{
  SupDtoModel model;  // has one container by default

  QTabWidget tab_widget;
  const DtoComposerTabController controller(&model, CreateCallback(), &tab_widget);

  EXPECT_EQ(tab_widget.count(), 1);

  auto new_root = mvvm::utils::CreateEmptyRootItem();
  new_root->InsertItem<mvvm::ContainerItem>(mvvm::TagIndex::Append());
  new_root->InsertItem<mvvm::ContainerItem>(mvvm::TagIndex::Append());

  model.ReplaceRootItem(std::move(new_root));
  EXPECT_EQ(tab_widget.count(), 2);
}
