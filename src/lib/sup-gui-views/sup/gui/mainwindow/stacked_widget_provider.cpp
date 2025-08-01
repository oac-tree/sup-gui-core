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

#include "stacked_widget_provider.h"

#include <QStackedWidget>

namespace sup::gui
{

StackedWidgetProvider::StackedWidgetProvider(QStackedWidget *stacked_widget)
    : m_stacked_widget(stacked_widget)
{
  if (!m_stacked_widget)
  {
    throw RuntimeException("Widget stack is not initialised");
  }
}

QStackedWidget *StackedWidgetProvider::GetStackedWidget() const
{
  return m_stacked_widget;
}

QWidget *StackedWidgetProvider::SetCurrentWidget(const std::string &key)
{
  auto iter = m_key_to_widget.find(key);
  if (iter == m_key_to_widget.end())
  {
    auto factory_func = GetFactoryFunc(key);
    auto widget = factory_func();
    auto index = m_stacked_widget->addWidget(widget.release());
    m_stacked_widget->setCurrentIndex(index);
    m_key_to_widget.insert({key, m_stacked_widget->currentWidget()});
  }
  else
  {
    m_stacked_widget->setCurrentWidget(iter->second);
  }

  return m_stacked_widget->currentWidget();
}

StackedWidgetProvider::widget_factory_func_t StackedWidgetProvider::GetFactoryFunc(
    const std::string &key) const
{
  auto iter = m_key_to_factory_func.find(key);
  if (iter == m_key_to_factory_func.end())
  {
    if (m_default_widget_factory_func)
    {
      return m_default_widget_factory_func;
    }
    throw RuntimeException("Key is not registered");
  }

  return iter->second;
}

}  // namespace sup::gui
