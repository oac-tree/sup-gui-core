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

#include "app_context_manager.h"

#include "app_context.h"

#include <sup/gui/core/exceptions.h>

#include <mvvm/core/unique_id_generator.h>

#include <QWidget>

namespace sup::gui
{

AppContext AppContextManager::RegisterWidgetUniqueId(const QWidget *widget)
{
  auto iter = m_widget_to_context.find(widget);
  if (iter != m_widget_to_context.end())
  {
    return iter->second;
  }

  const auto unique_id = QString::fromStdString(mvvm::UniqueIdGenerator::Generate());
  const auto context_name = widget->metaObject()->className();

  AppContext context(unique_id, context_name);
  (void)m_widget_to_context.insert(iter, {widget, context});
  return context;
}

void AppContextManager::UnregisterWidgetUniqueId(const QWidget *widget)
{
  auto iter = m_widget_to_context.find(widget);
  if (iter == m_widget_to_context.end())
  {
    throw RuntimeException("This widget wasn't registered");
  }

  (void)m_widget_to_context.erase(iter);
}

AppContext AppContextManager::GetContext(const QWidget *widget) const
{
  auto iter = m_widget_to_context.find(widget);
  return iter == m_widget_to_context.end() ? AppContext{} : iter->second;
}

const QWidget *AppContextManager::GetWidget(const AppContext &context) const
{
  auto on_element = [&context](auto element) { return element.second == context; };
  auto iter =
      std::find_if(std::begin(m_widget_to_context), std::end(m_widget_to_context), on_element);
  return iter == m_widget_to_context.end() ? nullptr : iter->first;
}

bool AppContextManager::HasContext(const QWidget *widget) const
{
  auto iter = m_widget_to_context.find(widget);
  return iter != m_widget_to_context.end();
}

size_t AppContextManager::GetNumberOfRegistrations() const
{
  return m_widget_to_context.size();
}

}  // namespace sup::gui
