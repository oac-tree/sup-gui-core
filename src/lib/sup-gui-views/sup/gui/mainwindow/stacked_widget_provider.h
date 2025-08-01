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

#ifndef SUP_GUI_MAINWINDOW_STACKED_WIDGET_PROVIDER_H_
#define SUP_GUI_MAINWINDOW_STACKED_WIDGET_PROVIDER_H_

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

class QStackedWidget;
class QWidget;

namespace sup::gui
{

/**
 * @brief The StackedWidgetProvider class populates QStackedWidget with content depending on the
 * given context.
 *
 * It is used in the context of SettingsEditor when we want to show/edit a certain SessionItem using
 * various custom-designed widgets.
 *
 * The user should first register the widget factory function for the given key. When necessary, the
 * provider will create widgets and add them to a stack. If a widget of this type was already
 * created before, it will be poped up.
 */
class StackedWidgetProvider
{
public:
  using widget_factory_func_t = std::function<std::unique_ptr<QWidget>()>;
  explicit StackedWidgetProvider(QStackedWidget* stacked_widget);

  /**
   * @brief Returns the widget stack which is under control.
   */
  QStackedWidget* GetStackedWidget() const ;

  /**
   * @brief Registers widget factory function for given key.
   */
  template <typename WidgetT>
  void RegisterWidget(const std::string& key);

  /**
   * @brief Show widget registered for given key.
   *
   * If widget doesn't exist, it will be created first using factory function.
   */
  QWidget* SetCurrentWidget(const std::string& key);

  /**
   * @brief Returns factory function registered for given key.
   */
  widget_factory_func_t GetFactoryFunc(const std::string& key) const;

private:
  QStackedWidget* m_stacked_widget{nullptr};
  std::map<std::string, widget_factory_func_t> m_key_to_factory_func;
  std::map<std::string, QWidget*> m_key_to_widget;
};

template <typename WidgetT>
void StackedWidgetProvider::RegisterWidget(const std::string& key)
{
  auto iter = m_key_to_factory_func.find(key);
  if (iter != m_key_to_factory_func.end())
  {
    throw RuntimeException("Already existing key");
  }
  m_key_to_factory_func.insert({key, []() { return std::make_unique<WidgetT>(); }});
}

}  // namespace sup::gui

#endif  // SUP_GUI_MAINWINDOW_STACKED_WIDGET_CONTROLLER_H_
