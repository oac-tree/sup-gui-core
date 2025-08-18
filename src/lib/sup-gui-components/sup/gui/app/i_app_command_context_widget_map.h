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

#ifndef SUP_GUI_APP_I_COMMAND_CONTEXT_WIDGET_MAP_H_
#define SUP_GUI_APP_I_COMMAND_CONTEXT_WIDGET_MAP_H_

#include <sup/gui/app/app_command_context.h>

class QWidget;

#include <cstddef>

namespace sup::gui
{

/**
 * @brief The IAppCommandContextWidgetMap interface is intended to handle the correspondance of
 * focus widgets and AppCommandContext.
 */
class IAppCommandContextWidgetMap
{
public:
  IAppCommandContextWidgetMap() = default;
  virtual ~IAppCommandContextWidgetMap() = default;

  IAppCommandContextWidgetMap(const IAppCommandContextWidgetMap&) = delete;
  IAppCommandContextWidgetMap& operator=(const IAppCommandContextWidgetMap&) = delete;
  IAppCommandContextWidgetMap(IAppCommandContextWidgetMap&&) = delete;
  IAppCommandContextWidgetMap& operator=(IAppCommandContextWidgetMap&&) = delete;

  /**
   * @brief Registers given widget using unique identifier and returns associated context to the
   * user.
   *
   * If widget was already registered, return corresponding conext.
   */
  virtual AppCommandContext RegisterWidgetUniqueId(const QWidget* widget) = 0;

  /**
   * @brief Unregisters given widget.
   *
   * Associated widget's unique context will bre removed from the global database. It is assumed,
   * that the widget was registered earlier with AppRegisterWidgetUniqueId call. This method
   * normally should be called in the destructor of the widget.
   */
  virtual void UnregisterWidgetUniqueId(const QWidget* widget) = 0;

  /**
   * @brief Returns context for widget.
   *
   * Context will be empty, if widget wasn't registered yet.
   */
  virtual AppCommandContext GetContext(const QWidget* widget) const = 0;

  /**
   * @brief Returns registered widget for give context.
   */
  virtual const QWidget* GetWidget(const AppCommandContext& context) const = 0;

  /**
   * @brief Checks if widget was registered.
   */
  virtual bool HasContext(const QWidget* widget) const = 0;

  /**
   * @brief Returns a number of registered widgets.
   */
  virtual std::size_t GetNumberOfRegistrations() const = 0;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_I_COMMAND_CONTEXT_WIDGET_MAP_H_
