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

#ifndef SUP_GUI_APP_NULL_COMMAND_SERVICE_H_
#define SUP_GUI_APP_NULL_COMMAND_SERVICE_H_

#include <sup/gui/app/i_app_command_service.h>

namespace sup::gui
{

class NullCommandService : public IAppCommandService
{
public:
  NullCommandService() = default;

  AppCommandContext RegisterWidgetUniqueId(const QWidget* widget) override;

  void UnregisterWidgetUniqueId(const QWidget* widget) override;

  QMenu* AppGetMenu(const QString& menu_name) override;

  bool AddActionToMenuBar(const QString& menu_name, QAction* action) override;

  AppCommand* AddCommandToMenu(const QString& menu_name, const QString& command_id) override;

  bool AddActionToCommand(QAction* action, const QString& command_id,
                          const AppCommandContext& context) override;

  AppCommand* GetCommand(const QString& command_id) override;
};

}  // namespace sup::gui

#endif  // SUP_GUI_APP_I_APP_COMMAND_SERVICE_H_
