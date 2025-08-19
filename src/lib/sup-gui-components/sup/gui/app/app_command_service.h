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

#ifndef SUP_GUI_APP_APP_COMMAND_SERVICE_H_
#define SUP_GUI_APP_APP_COMMAND_SERVICE_H_

#include <sup/gui/app/i_app_command_service.h>

#include <memory>

namespace sup::gui
{

class IAppCommandRegistry;
class IAppCommandContextWidgetMap;
class IActionContainerRegistry;
class AppContextFocusController;

class AppCommandService : public IAppCommandService
{
public:
  /**
   * @brief Main c-tor.
   *
   * @param command_registry The registry with application-wide commands.
   * @param context_widget_map The registry with widgets and their associated context ids.
   * @param menubar_action_registry The registry with application-wide menubar actions.
   */
  AppCommandService(std::unique_ptr<IAppCommandRegistry> command_registry,
                    std::unique_ptr<IAppCommandContextWidgetMap> context_widget_map,
                    std::unique_ptr<IActionContainerRegistry> menubar_action_registry);
  ~AppCommandService() override;

  AppCommandService(const AppCommandService&) = delete;
  AppCommandService& operator=(const AppCommandService&) = delete;
  AppCommandService(AppCommandService&&) = delete;
  AppCommandService& operator=(AppCommandService&&) = delete;

  void AppRegisterMenuBar(QMenuBar* menubar, const QStringList& top_menu_names) override;

  AppCommandContext RegisterWidgetUniqueId(const QWidget* widget) override;

  void UnregisterWidgetUniqueId(const QWidget* widget) override;

  QMenu* AppGetMenu(const QString& menu_name) override;

  bool AddActionToMenuBar(const QString& menu_name, QAction* action) override;

  AppCommand* AddCommandToMenu(const QString& menu_name, const QString& command_id) override;

  bool AddActionToCommand(QAction* action, const QString& command_id,
                          const AppCommandContext& context) override;

  AppCommand* GetCommand(const QString& command_id) override;

  void OnFocusWidgetUpdate(QWidget* old, QWidget* now) override;

private:
  std::unique_ptr<IAppCommandRegistry> m_command_registry;
  std::unique_ptr<IAppCommandContextWidgetMap> m_command_context_widget_map;
  std::unique_ptr<IActionContainerRegistry> m_app_menubar_action_registry;
  std::unique_ptr<AppContextFocusController> m_focus_controller;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_I_APP_COMMAND_SERVICE_H_
