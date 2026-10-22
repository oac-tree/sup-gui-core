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

#include "sup/gui/app/app_context_focus_controller.h"

#include <sup/gui/app/app_command.h>
#include <sup/gui/app/app_command_context.h>
#include <sup/gui/app/app_command_context_widget_map.h>
#include <sup/gui/app/app_command_registry.h>
#include <sup/gui/component_objects/proxy_action.h>

#include <gtest/gtest.h>

#include <QAction>
#include <QWidget>

namespace sup::gui::test
{

class AppContextFocusControllerTest : public ::testing::Test
{
};

//! Registering context for widget, and action for the same context.
//! Checking that AppContextFocusController::OnFocusWidgetUpdate properly sets proxy action.
TEST_F(AppContextFocusControllerTest, SingleWidget)
{
  // registering widget for certain context
  QWidget widget;
  AppCommandContextWidgetMap context_widget_map;
  auto context = context_widget_map.RegisterWidgetUniqueId(&widget);

  // registering action for the same context
  QAction action;
  AppCommandRegistry command_registry;
  const QString command_id("command-id");
  auto command = command_registry.RegisterAction(&action, command_id, context);

  // command's underlying proxy action is pointing nowhere
  EXPECT_EQ(command->GetProxyAction()->GetAction(), nullptr);

  // preparing focus controller
  AppContextFocusController focus_controller(context_widget_map, command_registry);

  // pretending widget got a focus
  focus_controller.OnFocusWidgetUpdate(nullptr, &widget);

  // command's underlying proxy action should point to the real action
  EXPECT_EQ(command->GetProxyAction()->GetAction(), &action);

  // pretending another widget got a focus
  QWidget another_widget;
  focus_controller.OnFocusWidgetUpdate(&widget, &another_widget);

  // command's underlying proxy action is pointing nowhere
  EXPECT_EQ(command->GetProxyAction()->GetAction(), nullptr);
}

//! Focus moves back to the widget while the commands are updated, as when a focused button gets
//! disabled. All commands should follow the latest focus widget (real-life bug).
TEST_F(AppContextFocusControllerTest, NestedFocusChange)
{
  QWidget widget;
  QWidget outside_widget;
  AppCommandContextWidgetMap context_widget_map;
  auto context = context_widget_map.RegisterWidgetUniqueId(&widget);

  QAction action0;
  QAction action1;
  AppCommandRegistry command_registry;
  auto command0 = command_registry.RegisterAction(&action0, "command0", context);
  auto command1 = command_registry.RegisterAction(&action1, "command1", context);

  AppContextFocusController focus_controller(context_widget_map, command_registry);
  focus_controller.OnFocusWidgetUpdate(nullptr, &widget);
  ASSERT_EQ(command0->GetProxyAction()->GetAction(), &action0);
  ASSERT_EQ(command1->GetProxyAction()->GetAction(), &action1);

  // the first proxy action change moves the focus back to the widget
  bool focus_moved_back{false};
  auto on_proxy_changed = [&focus_moved_back, &focus_controller, &widget, &outside_widget]()
  {
    if (!focus_moved_back)
    {
      focus_moved_back = true;
      focus_controller.OnFocusWidgetUpdate(&outside_widget, &widget);
    }
  };
  (void)QObject::connect(command0->GetProxyAction(), &QAction::changed, on_proxy_changed);
  (void)QObject::connect(command1->GetProxyAction(), &QAction::changed, on_proxy_changed);

  focus_controller.OnFocusWidgetUpdate(&widget, &outside_widget);

  EXPECT_TRUE(focus_moved_back);
  EXPECT_EQ(command0->GetProxyAction()->GetAction(), &action0);
  EXPECT_EQ(command1->GetProxyAction()->GetAction(), &action1);
}

}  // namespace sup::gui::test
