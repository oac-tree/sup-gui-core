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

#include "sup/gui/components/dto_composer_action_handler.h"

#include <sup/gui/core/exceptions.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/container_item.h>

#include <sup/dto/anytype.h>

#include <gtest/gtest.h>

using namespace sup::gui;

class DtoComposerActionHandlerTest : public ::testing::Test
{
public:
  mvvm::ApplicationModel m_model;
};

TEST_F(DtoComposerActionHandlerTest, OnAddNewContainer)
{
  DtoComposerActionHandler action_handler(&m_model);

  action_handler.OnAddNewContainer();
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
}

TEST_F(DtoComposerActionHandlerTest, OnAddNewContainerWhenModelIsEmpty)
{
  DtoComposerActionHandler action_handler(nullptr);

  EXPECT_THROW(action_handler.OnAddNewContainer(), RuntimeException);

  action_handler.SetModel(&m_model);

  action_handler.OnAddNewContainer();
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
}

TEST_F(DtoComposerActionHandlerTest, OnRemoveContainer)
{
  auto container0 = m_model.InsertItem<mvvm::ContainerItem>();
  auto container1 = m_model.InsertItem<mvvm::ContainerItem>();
  auto container2 = m_model.InsertItem<mvvm::ContainerItem>();

  DtoComposerActionHandler action_handler(&m_model);

  action_handler.OnRemoveContainer(1);
  EXPECT_EQ(m_model.GetRootItem()->GetAllItems(),
            std::vector<mvvm::SessionItem*>({container0, container2}));
}

TEST_F(DtoComposerActionHandlerTest, OnDuplicateContainer)
{
  auto container0 = m_model.InsertItem<mvvm::ContainerItem>();
  auto container1 = m_model.InsertItem<mvvm::ContainerItem>();

  auto scalar_item = container0->InsertItem<AnyValueScalarItem>(mvvm::TagIndex::Append());
  scalar_item->SetAnyTypeName(sup::dto::kInt8TypeName);
  scalar_item->SetData(mvvm::int8{42});

  DtoComposerActionHandler action_handler(&m_model);
  action_handler.OnDuplicateContainer(0);

  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 3);

  auto container2 = m_model.GetRootItem()->GetItem(mvvm::TagIndex::Default(1));
  auto copied_scalar = container2->GetItem<AnyValueScalarItem>(mvvm::TagIndex::Default(0));
  EXPECT_EQ(copied_scalar->Data<mvvm::int8>(), 42);
}
