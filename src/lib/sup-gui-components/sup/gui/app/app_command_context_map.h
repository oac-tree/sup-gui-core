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

#ifndef SUP_GUI_APP_APP_COMMAND_CONTEXT_MAP_H_
#define SUP_GUI_APP_APP_COMMAND_CONTEXT_MAP_H_

#include <map>

class QWidget;

namespace sup::gui
{

class AppCommandContext;

/**
 * @brief The AppCommandContextMap class stores correspondence of focus widgets to
 * AppCommandContext.
 */
class AppCommandContextMap
{
public:
  /**
   * @brief Registers given widget using unique identifier and returns associated context to the
   * user.
   *
   * If widget was already registered, return corresponding conext.
   */
  AppCommandContext RegisterWidgetUniqueId(const QWidget* widget);

  /**
   * @brief Unregisters given widget.
   *
   * Associated widget's unique context will bre removed from the global database. It is assumed,
   * that the widget was registered earlier with AppRegisterWidgetUniqueId call. This method
   * normally should be called in the destructor of the widget.
   */
  void UnregisterWidgetUniqueId(const QWidget* widget);

  /**
   * @brief Returns context for widget.
   *
   * Context will be empty, if widget wasn't registered yet.
   */
  AppCommandContext GetContext(const QWidget* widget) const;

  /**
   * @brief Returns registered widget for give context.
   */
  const QWidget* GetWidget(const AppCommandContext& context) const;

  /**
   * @brief Checks if widget was registered.
   */
  bool HasContext(const QWidget* widget) const;

  /**
   * @brief Returns a number of registered widgets.
   */
  std::size_t GetNumberOfRegistrations() const;

private:
  std::map<const QWidget*, AppCommandContext> m_widget_to_context;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_APP_COMMAND_CONTEXT_MAP_H_
