/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "sup/gui/app/app_command_service.h"

#include <sup/gui/app/app_command.h>
#include <sup/gui/app/menu_action_container.h>
#include <sup/gui/components/proxy_action.h>
#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <gtest/gtest.h>
#include <testutils/mock_action_container_registry.h>
#include <testutils/mock_app_command_context_widget_map.h>
#include <testutils/mock_app_command_registry.h>

#include <QMenu>
#include <QWidget>

namespace sup::gui::test
{

/**
 * @brief Tests of AppCommandService class.
 */
class AppCommandServiceTest : public ::testing::Test
{
public:
  /**
   * @brief Creates command service and wires it with mock components.
   */
  std::unique_ptr<AppCommandService> CreateCommandService()
  {
    auto command_registry = std::make_unique<AppCommandRegistryDecorator>(&m_command_registry);
    auto action_container_registry =
        std::make_unique<ActionContainerRegistryDecorator>(&m_action_container_registry);
    auto widget_map = std::make_unique<AppCommandContextWidgetMapDecorator>(&m_widget_map);
    return std::make_unique<AppCommandService>(std::move(command_registry), std::move(widget_map),
                                               std::move(action_container_registry));
  };

  MockAppCommandRegistry m_command_registry;
  MockAppCommandContextWidgetMap m_widget_map;
  MockActionContainerRegistry m_action_container_registry;
};

TEST_F(AppCommandServiceTest, RegisterWidget)
{
  auto service = CreateCommandService();
  const QWidget widget;
  const AppCommandContext context("test_conetxt");

  ON_CALL(m_widget_map, RegisterWidgetUniqueId(&widget)).WillByDefault(::testing::Return(context));
  EXPECT_CALL(m_widget_map, RegisterWidgetUniqueId(&widget)).WillOnce(testing::Return(context));

  auto registered_context = service->RegisterWidgetUniqueId(&widget);
  EXPECT_EQ(registered_context, context);
}

TEST_F(AppCommandServiceTest, UnregisterWidget)
{
  auto service = CreateCommandService();
  const QWidget widget;
  const AppCommandContext context("test_conetxt");

  EXPECT_CALL(m_widget_map, UnregisterWidgetUniqueId(&widget)).Times(1);

  service->UnregisterWidgetUniqueId(&widget);
}

TEST_F(AppCommandServiceTest, AppGetMenu)
{
  auto service = CreateCommandService();
  const QString menu_name = "File";
  auto menu = std::make_unique<QMenu>(menu_name);

  MenuActionContainer container(menu_name, menu.get());

  ON_CALL(m_action_container_registry, GetContainer(menu_name))
      .WillByDefault(testing::Return(&container));
  EXPECT_CALL(m_action_container_registry, GetContainer(menu_name))
      .WillOnce(testing::Return(&container));

  auto retrieved_menu = service->AppGetMenu(menu_name);
  EXPECT_EQ(retrieved_menu, menu.get());
}

TEST_F(AppCommandServiceTest, AddActionToMenuBar)
{
  auto service = CreateCommandService();
  const QString menu_name = "File";
  QAction action;

  EXPECT_CALL(m_action_container_registry, RegisterAction(menu_name, &action))
      .WillOnce(testing::Return(true));

  const bool result = service->AddActionToMenuBar(menu_name, &action);
  EXPECT_TRUE(result);
}

TEST_F(AppCommandServiceTest, AddCommandToMenu)
{
  auto service = CreateCommandService();
  const QString menu_name = "File";
  const QString command_id = "test_command";

  auto command = std::make_unique<AppCommand>(command_id);
  ON_CALL(m_command_registry, RegisterCommand(command_id, command_id))
      .WillByDefault(testing::Return(command.get()));
  EXPECT_CALL(m_command_registry, RegisterCommand(command_id, command_id))
      .WillOnce(testing::Return(command.get()));

  EXPECT_CALL(m_action_container_registry, RegisterAction(menu_name, command->GetProxyAction()))
      .WillOnce(testing::Return(true));

  auto registered_command = service->AddCommandToMenu(menu_name, command_id);
  EXPECT_EQ(registered_command, command.get());
}

TEST_F(AppCommandServiceTest, AddCommandToMenuFails)
{
  auto service = CreateCommandService();
  const QString menu_name = "File";
  const QString command_id = "test_command";

  auto command = std::make_unique<AppCommand>(command_id);
  ON_CALL(m_command_registry, RegisterCommand(command_id, command_id))
      .WillByDefault(testing::Return(command.get()));
  EXPECT_CALL(m_command_registry, RegisterCommand(command_id, command_id))
      .WillOnce(testing::Return(command.get()));

  EXPECT_CALL(m_action_container_registry, RegisterAction(menu_name, command->GetProxyAction()))
      .WillOnce(testing::Return(false));

  EXPECT_THROW(service->AddCommandToMenu(menu_name, command_id), RuntimeException);
}

TEST_F(AppCommandServiceTest, ActionToCommand)
{
  auto service = CreateCommandService();
  const QString command_id = "test_command";
  const AppCommandContext context("test_context");
  QAction action;

  auto command = std::make_unique<AppCommand>(command_id);
  ON_CALL(m_command_registry, RegisterAction(&action, command_id, context))
      .WillByDefault(testing::Return(command.get()));
  EXPECT_CALL(m_command_registry, RegisterAction(&action, command_id, context))
      .WillOnce(testing::Return(command.get()));

  const bool result = service->AddActionToCommand(&action, command_id, context);
  EXPECT_TRUE(result);
}

TEST_F(AppCommandServiceTest, AddActionToCommandFailure)
{
  auto service = CreateCommandService();
  const QString command_id = "test_command";
  const AppCommandContext context("test_context");
  QAction action;

  ON_CALL(m_command_registry, RegisterAction(&action, command_id, context))
      .WillByDefault(testing::Return(nullptr));
  EXPECT_CALL(m_command_registry, RegisterAction(&action, command_id, context))
      .WillOnce(testing::Return(nullptr));

  const bool result = service->AddActionToCommand(&action, command_id, context);
  EXPECT_FALSE(result);
}

}  // namespace sup::gui::test
