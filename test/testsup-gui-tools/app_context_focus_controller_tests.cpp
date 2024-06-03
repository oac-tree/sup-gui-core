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

#include "sup/gui/app/app_context_focus_controller.h"

#include <sup/gui/app/app_command.h>
#include <sup/gui/app/app_command_manager.h>
#include <sup/gui/app/app_context.h>
#include <sup/gui/app/app_context_manager.h>
#include <sup/gui/widgets/proxy_action.h>

#include <gtest/gtest.h>

#include <QAction>
#include <QWidget>

using namespace sup::gui;

class AppContextFocusControllerTest : public ::testing::Test
{
};

//! Registering context for widget, and action for the same context.
//! Checking that AppContextFocusController::OnFocusWidgetUpdate properly sets proxy action.
TEST_F(AppContextFocusControllerTest, SingleWidget)
{
  // registering widget for certain context
  QWidget widget;
  AppContextManager context_manager;
  auto context = context_manager.RegisterWidgetUniqueId(&widget);

  // registering action for the same context
  QAction action;
  AppCommandManager command_manager;
  const QString command_id("command-id");
  auto command = command_manager.RegisterAction(&action, command_id, context);

  // command's underlying proxy action is pointing nowhere
  EXPECT_EQ(command->GetProxyAction()->GetAction(), nullptr);

  // preparing focus controller
  AppContextFocusController focus_controller(context_manager, command_manager, nullptr);

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
