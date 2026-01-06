/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#ifndef TESTUTILS_MOCK_ACTION_CONTAINER_REGISTRY_H_
#define TESTUTILS_MOCK_ACTION_CONTAINER_REGISTRY_H_

#include <sup/gui/app/i_action_container_registry.h>

#include <gmock/gmock.h>

namespace sup::gui::test
{

/**
 * @brief The MockActionContainerRegistry is a mock to test IActionContainerRegistry interface.
 */
class MockActionContainerRegistry : public IActionContainerRegistry
{
public:
  MOCK_METHOD(void, SetMenuBar, (QMenuBar * menubar), (override));
  MOCK_METHOD(IActionContainer*, AddMenu, (const QString& menu_name), (override));
  MOCK_METHOD(bool, RegisterAction, (const QString& container_name, QAction* action), (override));
  MOCK_METHOD(IActionContainer*, GetContainer, (const QString& container_name), (override));
};

/**
 * @brief The ActionContainerRegistryDecorator is a decorator for IActionContainerRegistry
 * inetrface.
 */
class ActionContainerRegistryDecorator : public IActionContainerRegistry
{
public:
  explicit ActionContainerRegistryDecorator(IActionContainerRegistry* registry);

  void SetMenuBar(QMenuBar* menubar) override;

  IActionContainer* AddMenu(const QString& menu_name) override;

  bool RegisterAction(const QString& container_name, QAction* action) override;

  IActionContainer* GetContainer(const QString& container_name) override;

private:
  IActionContainerRegistry* m_registry{nullptr};
};

}  // namespace sup::gui::test

#endif  // TESTUTILS_MOCK_ACTION_CONTAINER_REGISTRY_H_
