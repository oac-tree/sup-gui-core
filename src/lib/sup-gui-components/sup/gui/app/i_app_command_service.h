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

#ifndef SUP_GUI_APP_I_APP_COMMAND_SERVICE_H_
#define SUP_GUI_APP_I_APP_COMMAND_SERVICE_H_

#include <QVector>

class QWidget;
class QString;
class QMenuBar;

namespace sup::gui
{

class AppCommand;
class AppCommandContext;

/**
 * @brief The IAppCommandService interface allows to register commands, subscribe multiple actions
 * to it and execute some of them depending on the current context.
 *
 * A command (AppCommand class) is a proxy action that can be forwarded to other actions depending
 * on the current context.
 *
 * A context (AppCommandContext) is an unique itendifier for widget. When widget is in focus,
 */
class IAppCommandService
{
public:
  IAppCommandService() = default;
  virtual ~IAppCommandService() = default;

  IAppCommandService(const IAppCommandService&) = delete;
  IAppCommandService& operator=(const IAppCommandService&) = delete;
  IAppCommandService(IAppCommandService&&) = delete;
  IAppCommandService& operator=(IAppCommandService&&) = delete;

  /**
   * @brief Registers given widget using unique identifier.
   *
   * This call generates a unique identifier (represented by AppContext class) and registers the
   * widget in the global database.
   *
   * @return The context representing used unique identifier.
   */
  virtual AppCommandContext RegisterWidgetUniqueId(const QWidget* widget) = 0;

  /**
   * @brief Unregisters given widget.
   *
   * Associated widget's unique context will be removed from the global database. It is assumed,
   * that the widget was registered earlier with AppRegisterWidgetUniqueId call. This method
   * normally should be called in the destructor of the widget.
   */
  virtual void UnregisterWidgetUniqueId(const QWidget* widget) = 0;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_I_APP_COMMAND_SERVICE_H_
