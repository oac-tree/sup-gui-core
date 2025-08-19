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

#include "mock_app_command_context_widget_map.h"

namespace sup::gui::test
{

AppCommandContext AppCommandContextWidgetMapDecorator::RegisterWidgetUniqueId(const QWidget *widget)
{
  return m_map->RegisterWidgetUniqueId(widget);
}

void AppCommandContextWidgetMapDecorator::UnregisterWidgetUniqueId(const QWidget *widget)
{
  m_map->UnregisterWidgetUniqueId(widget);
}

AppCommandContext AppCommandContextWidgetMapDecorator::GetContext(const QWidget *widget) const
{
  return m_map->GetContext(widget);
}

const QWidget *AppCommandContextWidgetMapDecorator::GetWidget(
    const AppCommandContext &context) const
{
  return m_map->GetWidget(context);
}

bool AppCommandContextWidgetMapDecorator::HasContext(const QWidget *widget) const
{
  return m_map->HasContext(widget);
}

std::size_t AppCommandContextWidgetMapDecorator::GetNumberOfRegistrations() const
{
  return m_map->GetNumberOfRegistrations();
}

}  // namespace sup::gui::test
