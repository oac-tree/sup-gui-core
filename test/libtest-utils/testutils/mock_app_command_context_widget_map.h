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

#ifndef TESTUTILS_MOCK_APP_COMMAND_CONTEXT_WIDGET_MAP_H_
#define TESTUTILS_MOCK_APP_COMMAND_CONTEXT_WIDGET_MAP_H_

#include <sup/gui/app/i_app_command_context_widget_map.h>

#include <gmock/gmock.h>

namespace sup::gui::test
{

/**
 * @brief The MockAppCommandContextWidgetMap is a mock to test IAppCommandContextWidgetMap
 * inetrface.
 */
class MockAppCommandContextWidgetMap : public IAppCommandContextWidgetMap
{
public:
  MOCK_METHOD(AppCommandContext, RegisterWidgetUniqueId, (const QWidget* widget), (override));
  MOCK_METHOD(void, UnregisterWidgetUniqueId, (const QWidget* widget), (override));
  MOCK_METHOD(AppCommandContext, GetContext, (const QWidget*), (const, override));
  MOCK_METHOD(const QWidget*, GetWidget, (const AppCommandContext& context), (const, override));
  MOCK_METHOD(bool, HasContext, (const QWidget*), (const, override));
  MOCK_METHOD(std::size_t, GetNumberOfRegistrations, (), (const, override));
};

/**
 * @brief The AppCommandContextWidgetMapDecorator class is a decorator for
 * IAppCommandContextWidgetMap.
 */
class AppCommandContextWidgetMapDecorator : public IAppCommandContextWidgetMap
{
public:
  explicit AppCommandContextWidgetMapDecorator(IAppCommandContextWidgetMap* map);

  AppCommandContext RegisterWidgetUniqueId(const QWidget* widget) override;

  void UnregisterWidgetUniqueId(const QWidget* widget) override;

  AppCommandContext GetContext(const QWidget* widget) const override;

  const QWidget* GetWidget(const AppCommandContext& context) const override;

  bool HasContext(const QWidget* widget) const override;

  std::size_t GetNumberOfRegistrations() const override;

private:
  IAppCommandContextWidgetMap* m_map{nullptr};
};
}  // namespace sup::gui::test

#endif  // TESTUTILS_MOCK_APP_COMMAND_CONTEXT_WIDGET_MAP_H_
