/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_ANYVALUE_EDITOR_CONTEXT_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_ANYVALUE_EDITOR_CONTEXT_H_

#include <sup/gui/app/app_command_context.h>
#include <sup/gui/app/i_app_command_registry.h>

#include <gmock/gmock.h>

namespace sup::gui::test
{

/**
 * @brief The MockAppCommandRegistry is a mocking class to test IAppcommandRegistry interface.
 */
class MockAppCommandRegistry : public IAppCommandRegistry
{
public:
  MOCK_METHOD(AppCommand*, RegisterCommand,
              (const QString& command_id, const QString& command_text), (override));

  MOCK_METHOD(AppCommand*, RegisterAction,
              (QAction * action, const QString& command_id, const AppCommandContext& context),
              (override));

  MOCK_METHOD(AppCommand*, GetCommand, (const QString& command_id), (override));

  MOCK_METHOD(std::size_t, GetCommandCount, (), (const, override));

  MOCK_METHOD(void, SetCurrentContext, (const AppCommandContext& context), (override));

  MOCK_METHOD(void, SetContextStack, (const std::vector<AppCommandContext>&), (override));
};

/**
 * @brief The AppCommandRegistryDecorator class is a decorator for IAppCommandRegistry.
 */
class AppCommandRegistryDecorator : public IAppCommandRegistry
{
public:
  explicit AppCommandRegistryDecorator(IAppCommandRegistry* registry) : m_registry(registry) {}

  AppCommand* RegisterCommand(const QString& command_id, const QString& command_text) override;

  AppCommand* RegisterAction(QAction* action, const QString& command_id,
                             const AppCommandContext& context) override;

  AppCommand* GetCommand(const QString& command_id) override;

  std::size_t GetCommandCount() const override;

  void SetCurrentContext(const AppCommandContext& context) override;

  void SetContextStack(const std::vector<AppCommandContext>& contexts) override;

private:
  IAppCommandRegistry* m_registry{nullptr};
};

}  // namespace sup::gui::test

#endif
