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

#include <mvvm/core/unique_id_generator.h>

namespace sup::gui
{

void AppContextManager::RegisterContext(const QWidget *widget, const AppContext &context)
{
  auto iter = m_widget_to_context.find(widget);
  if (iter != m_widget_to_context.end())
  {
    iter->second.push_back(context);
    return;
  }

  m_widget_to_context.insert(iter, {widget, std::vector<AppContext>({context})});
}

AppContext AppContextManager::RegisterWidgetUniqueId(const QWidget *widget)
{
  AppContext context(QString::fromStdString(mvvm::UniqueIdGenerator::Generate()));
  RegisterContext(widget, context);
  return context;
}

std::vector<AppContext> AppContextManager::GetContext(const QWidget *widget) const
{
  auto iter = m_widget_to_context.find(widget);
  return iter != m_widget_to_context.end() ? iter->second : std::vector<AppContext>();
}

}  // namespace sup::gui
